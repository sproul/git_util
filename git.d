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
git status --short
exit
bx $dp/git_util/git.d a