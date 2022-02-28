:
t=/tmp/git.d.latest.`pwd | sed -e 's/[^a-zA-Z0-9]/_/g'`

all_mode=''
dry_mode=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                a|-all)
                        git.all $dry_mode git.d
                        exit
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

git diff . > $t
strait $t
git status --short > $t
if [ -s "$t" ]; then
        . modified_repos.inc
        git_util__modified_repos__Add_to_list $repo_name
fi
cat $t
exit
bx $dp/git_util/git.d a