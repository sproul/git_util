:
dry_mode=''
status_output_fn=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                -dry)
                        dry_mode=-dry
                ;;
                -q|-quiet)
                        verbose_mode=''
                ;;
                -status_output_fn)
                        shift
                        status_output_fn="$1"
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
if [ -f $status_output_fn ]; then
        cat $status_output_fn
else
        git status .
fi | grep '^	modified: ' | sed -e 's/.*modified: //'
