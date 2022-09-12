:
t=/tmp/git.d.latest.`pwd | sed -e 's/[^a-zA-Z0-9]/_/g'`

all_mode=''
dry_mode=''
pull_latest_updates_mode=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                a|-all)
                        git.all $dry_mode git.d
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
                -pull_latest_updates_if_no_local_changes)
                        pull_latest_updates_mode=-pull_latest_updates
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

git diff . > $t
strait $t

if [ -n "$pull_latest_updates_mode" ]; then
        proj=`pwd | sed -e 's;.*/;;'`
        git status --short  > $t.status
        case $proj in
                bin)
                        if [ -n "grep Linux/carson.can $t.status" ]; then
                                echo.clean "git add Linux/carson.can*"
                                git             add Linux/carson.can*
                                echo.clean "git commit -m 'clojure notes'"
                                git             commit -m 'clojure notes' Linux/carson.*
                        fi
                ;;
                emacs)
                        if [ -n "grep lisp/data/n-data-menu-browse $t.status" ]; then
                                echo.clean "git commit -m 'shortcut updates' lisp/data/n-data-menu-browse.*"
                                git             commit -m 'shortcut updates' lisp/data/n-data-menu-browse.*
                        fi
                ;;
        esac
        rm $t.status
fi
. modified_repos.inc
git status --short > $t
if [ -s "$t" ]; then
        git_util__modified_repos__Add_to_list `pwd`
        rc=0
else
        git_util__modified_repos__Remove_from_list `pwd`
        if [ -n "$pull_latest_updates_mode" ]; then
                echo Since no local changes, will go ahead and sync to the latest checkin for `pwd`...
                git.up
        fi
        rc=1
fi
cat $t
exit $rc
exit
bx $dp/git_util/git.d a