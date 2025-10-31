:
t=/tmp/git.d.latest.`pwd | sed -e 's/[^a-zA-Z0-9]/_/g'`

Detect_and_maybe_correct_weird_config_disappearance()
{
	if [ ! -f .git/config ]; then
		echo "WARN confirmed no .git/config" 1>&2
		possible_fallback=$HOME/git/$proj/.git/config
		if [ -f $possible_fallback ]; then
                        if cp $possible_fallback .git/config; then
				echo "OK cp $possible_fallback .git/config" 1>&2
                                return
			else
				echo "FAIL cp $possible_fallback .git/config" 1>&2
                                exit 1
                        fi
                fi
                echo "FAIL could not find a replacement for .git/config" 1>&2
                exit 1
	fi
}


all_mode=''
dry_mode=''
terse_mode=''
verbose_mode=''
while [ -n "$1" ]; do
	case "$1" in
                -|-terse)
                        terse_mode=-terse
                ;;
		a|-all)
			for d in `cat $dp/.modified_repo_statuses`; do
				git.d -in_dir $d
			done
			exit
		;;
		-in_dir)
			shift
			dir="$1"
			if [ ! -d "$dir" ]; then
				echo "$0: error: could not find directory \"$dir\"" 1>&2
				exit 1
			fi
			cd "$dir"
		;;
		-dry)
			dry_mode=-dry
		;;
		-q|-quiet)
			verbose_mode=''
		;;
		-v|-verbose)
			verbose_mode=-v
		;;
		*)
			break
		;;
	esac
	shift
done
if [ -n "$1" ]; then
        sha=$1
        git show -s --format="%cd %B" --date=iso $sha
        git.status--short.for_sha $sha > $t.mid
        git diff $sha^ $sha . > $t
else
        case `pwd` in
                $dp/git/qwickanalytics_semi_structured_data*)
                        echo "WARN difr, not asking git" 1>&2
                        difr
                        exit 0
                ;;
        esac
        prune.midnight_files
        git_proj_dir=`ls.up -find_parent_of_dir .git`
        proj=`basename $git_proj_dir`
        cd $git_proj_dir
        Detect_and_maybe_correct_weird_config_disappearance
        git status --short > $t.mid
        if [ -n "$terse_mode" ]; then
                cat /dev/null > $t
        else
                git diff . > $t
        fi
fi
strait $t

. modified_repos.inc

if [ -s "$t.mid" ]; then
	git_util__modified_repos__Add_to_list $git_proj_dir
	rc=0
else
	git_util__modified_repos__Remove_from_list $git_proj_dir
	rc=1
fi
rm  $t

cat $t.mid
rm  $t.mid

exit $rc
exit
cd $dp/git/bin/
bx $dp/git/git_util/git.d 514b505484c632e21a617e76fc61d843acc0ac7c
