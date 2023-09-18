:
t=/tmp/git.d.latest.`pwd | sed -e 's/[^a-zA-Z0-9]/_/g'`

all_mode=''
dry_mode=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
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

git status --short > $t
midnight_files=`cat $t | grep '^ M .*/midnight[^\/]*$' | sed -e 's/^ M / /' | tr '\n' ' '`
if [ -n "$midnight_files" ]; then
        eat_2nd_and_later_lines $midnight_files > /dev/null 2>&1
fi

git diff . > $t
strait $t

. modified_repos.inc

git_proj_dir=`ls.up -find_parent_of_dir .git`
if [ -s "$t" ]; then
        git_util__modified_repos__Add_to_list $git_proj_dir
        rc=0
else
        git_util__modified_repos__Remove_from_list $git_proj_dir
        rc=1
fi
cat $t
exit $rc
exit
bx $dp/git_util/git.d a