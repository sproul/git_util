:
# cannot be used in isolation because it restores normal .git/config on process exit, so needs to be called as a function from the process
# which needs access (after whose exit the world will be restored to normal).

end_of_process_safe_gc_list=/tmp/git.prep_auth.inc.$$

Restore_clean_configs()
{
        if [ -f /tmp/Restore_clean_configs__just_return ]; then
                rm /tmp/Restore_clean_configs__just_return
                return
        fi
        if [ -f "$end_of_process_safe_gc_list" ]; then
                for safe_gc in `cat $end_of_process_safe_gc_list`; do
                        gc=`sed -e 's/\.[0-9][0-9]*$//' <<< $safe_gc`
                        if [ -f $safe_gc ]; then
                                mv $safe_gc $gc
                        fi
                done
                rm -f $end_of_process_safe_gc_list
        fi
}


Prep_auth()
{
        echo git.prep_auth.inc/Prep_auth
        if [ -z "$xrepo_name" ]; then
                xrepo_name=`basename $(pwd)`
        fi
        gc=.git/config
        safe_gc=$gc.$$
        if grep "url = https://sproul:" $gc > /dev/null 2>&1; then
                # left over from a previous run, but we don't want the gc value to sit here indefinitely
                cat $gc | sed -e "s;^\turl = https://sproul:.*;\turl = git@github.com:sproul/$xrepo_name.git;" > $safe_gc
                echo $safe_gc >> $end_of_process_safe_gc_list
                trap "Restore_clean_configs" EXIT
        elif grep "url = git@github.com:sproul/" $gc > /dev/null 2>&1; then
                if [ -f $HOME/.gh ]; then
                        # the normal case: temporarily insert gc value for this transaction
                        mv $gc $safe_gc
                        echo $safe_gc >> $end_of_process_safe_gc_list
                        trap "Restore_clean_configs" EXIT
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
