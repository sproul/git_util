:
debug_mode=''
dry_mode=''
t=`mktemp`; trap "rm $t*" EXIT
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                -dry)
                        dry_mode=-dry
                ;;
                -q|-quiet)
                        verbose_mode=''
                ;;
                -v|-verbose)
                        verbose_mode=-v
                ;;
                -x)
                        set -x
                        debug_mode=-x
                ;;
                -*)
                        echo "FAIL unrecognized flag $1" 1>&2
                        exit 1
                ;;
                *)
                        break
                ;;
        esac
        shift
done
target_sha=$1
if [ -z "$target_sha" ]; then
        echo "FAIL: expected a value for \"target_sha\" but saw nothing" 1>&2
        exit 1
fi
git log --pretty=format:"%H %cd" --date=iso | sed -e 's/^/ /' -e "s/ $target_sha/*$target_sha/" |
if [ -n "$verbose_mode" ]; then
        cat
else
        grep --context=8 $target_sha
fi
exit
$dp/git/git_util/git.sha_and_ts.ls 7b56ded4e99b4522c467004cc8cba0b2505ba315