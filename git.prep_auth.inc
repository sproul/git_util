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

Swap_gitconfigs()
{
        appropriate_gitconfig=$1
        if [ ! -f $HOME/.gitconfig.global ]; then
                echo.clean "cp $HOME/.gitconfig $HOME/.gitconfig.global"
                cp             $HOME/.gitconfig $HOME/.gitconfig.global
        fi
        if cmp $appropriate_gitconfig $HOME/.gitconfig > /dev/null 2>&1; then
                if [ -n "$verbose_mode" ]; then
                        echo "OK $appropriate_gitconfig $HOME/.gitconfig already match"
                fi
                return
        fi
        echo.clean "cp $appropriate_gitconfig $HOME/.gitconfig"
        cp             $appropriate_gitconfig $HOME/.gitconfig
        email=`grep 'email =' $HOME/.gitconfig | sed -e 's/.* = //'`
        if [ -f $HOME/.gh.$email ]; then
                cp $HOME/.gh.$email $HOME/.gh
        else
                cp $HOME/.gh.global $HOME/.gh
        fi
        . proxy_configure.sh
        if [ -z "$http_proxy" ]; then
                git config --global --unset http.proxy
        else
                git config --global         http.proxy $http_proxy
        fi
}

Prep_auth()
{
        echo git.prep_auth.inc/Prep_auth
        
        if [ -f .gitconfig.override ]; then
                Swap_gitconfigs .gitconfig
        elif [ -f $HOME/.gitconfig.global ]; then
                Swap_gitconfigs $HOME/.gitconfig.global
        fi
        
        if [ -z "$xrepo_name" ]; then
                xrepo_name=`basename $(pwd)`
        fi
        gc=.git/config
        safe_gc=$gc.$$
        if grep "url = https://.*sproul:" $gc > /dev/null 2>&1; then
                # left over from a previous run, but we don't want the gc value to sit here indefinitely
                #cat $gc | sed -e "s;^\turl = https://sproul:.*;\turl = git@github.com:sproul/$xrepo_name.git;" > $safe_gc
                cat  $gc | sed -e "s;^\turl = https://\\(.*sproul\\):.*;\turl = git@github.com:\\1/$xrepo_name.git;" > $safe_gc
                echo $safe_gc >> $end_of_process_safe_gc_list
                #trap "Restore_clean_configs" EXIT
        elif grep "url = git@github.com:.*sproul/" $gc > $t 2>&1; then
                gh_user=`cat $t | sed -e 's/.*github.com://' -e 's;/.*;;'`
                email=`grep 'email =' $HOME/.gitconfig | sed -e 's/.* = //'`
                if [ -f $HOME/.gh.$email ]; then
                        hgh=$HOME/.gh.$email
                elif [ -f $HOME/.gh ]; then
                        hgh=$HOME/.gh
                else
                        hgh=''
                fi
                if [ -n "$hgh" ]; then
                        # the normal case: temporarily insert gc value for this transaction
                        mv $gc $safe_gc
                        echo $safe_gc >> $end_of_process_safe_gc_list
                        #trap "Restore_clean_configs" EXIT
                        cat $safe_gc | sed -e "s;^\turl =.*;\turl = https://$gh_user:`cat $hgh`@github.com/$gh_user/$xrepo_name.git;" > $gc
                        grep @github.com $gc
                fi
        fi
        if [ -f $safe_gc ]; then
                if [ -n "$verbose_mode" ]; then
                        echo diff $safe_gc $gc
                        diff $safe_gc $gc
                fi
        fi
}
