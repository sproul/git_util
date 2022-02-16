:
# cannot be used in isolation because it restores normal .git/config on process exit, so needs to be called as a function from the process
# which needs access (after whose exit the world will be restored to normal).
Prep_auth()
{
        if [ -z "$xrepo_name" ]; then
                xrepo_name=`basename $(pwd)`
        fi
        gc=.git/config
        safe_gc=$gc.$$
        if grep "url = https://sproul:" $gc > /dev/null 2>&1; then
                # left over from a previous run, but we don't want the gc value to sit here indefinitely
                cat $gc | sed -e "s;^\turl = https://sproul:.*;\turl = git@github.com:sproul/$xrepo_name.git;" > $safe_gc
                trap "mv $safe_gc $gc" EXIT
        elif grep "url = git@github.com:sproul/" $gc > /dev/null 2>&1; then
                if [ -f $HOME/.gh ]; then
                        # the normal case: temporarily insert gc value for this transaction
                        mv $gc $safe_gc
                        trap "mv $safe_gc $gc" EXIT
                        cat $safe_gc | sed -e "s;^\turl =.*;\turl = https://sproul:`cat $HOME/.gh`@github.com/sproul/$xrepo_name.git;" > $gc
                fi
        fi
        if [ -f $safe_gc ]; then
                if [ -n "$verbose_mode" ]; then
                        echo diff $safe_gc $gc
                        diff $safe_gc $gc
                fi
        fi
}
Prep_auth
