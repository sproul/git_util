:
# cannot be used in isolation because it restores normal .git/config on process exit, so needs to be called as a function from the process
# which needs access (after whose exit the world will be restored to normal).

end_of_process_safe_gc_list=/tmp/git.prep_auth.inc.$$

Restore_clean_configs()
{
	find /tmp -type f -name 'git.prep_auth.inc*' -mtime +2 -delete > /dev/null 2>&1 &

	if [ -f /tmp/Restore_clean_configs__just_return ]; then
		rm /tmp/Restore_clean_configs__just_return
		rm -f $end_of_process_safe_gc_list
		return
	fi
	if [ -f "$end_of_process_safe_gc_list" ]; then
                for safe_gc in `sort -u $end_of_process_safe_gc_list`; do
			gc=`sed -e 's/\.[0-9][0-9]*$//' <<< $safe_gc`
			if [ -f $safe_gc ]; then





				if [ -n "disabled" ]; then
					echo Restore_clean_configs disabled
				else
					mv $safe_gc $gc
				fi





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
		cp	       $HOME/.gitconfig $HOME/.gitconfig.global
	fi
	if cmp $appropriate_gitconfig $HOME/.gitconfig > /dev/null 2>&1; then
		if [ -n "$verbose_mode" ]; then
			echo "OK $appropriate_gitconfig $HOME/.gitconfig already match"
		fi
		return
	fi
	echo.clean "cp $appropriate_gitconfig $HOME/.gitconfig"
	cp	       $appropriate_gitconfig $HOME/.gitconfig
	email=`grep 'email =' $HOME/.gitconfig | sed -e 's/.* = //'`
	if [ -f $HOME/.gh.$email ]; then
		cp $HOME/.gh.$email $HOME/.gh
	elif [ -f $HOME/.gh.$email ]; then
		cp $HOME/.gh.global $HOME/.gh
	fi
	. proxy_configure.sh
	if [ -z "$http_proxy" ]; then
		git.config1 --unset http.proxy
	else
		git.config1	    http.proxy $http_proxy
	fi
}

Prep_auth()
{
	echo git.prep_auth.inc/Prep_auth

	#
	# not clear why there are multiple versions. The one use case is orahub vs other git repos, but for a given machine, we should be
	# repeatedly interacting with the same repo
	#
	#if [ -f .gitconfig.override ]; then
	#	 Swap_gitconfigs .gitconfig.override
	#elif [ -f $HOME/.gitconfig.global ]; then
	#	 Swap_gitconfigs $HOME/.gitconfig.global
	#fi

	if [ -z "$xrepo_name" ]; then
		xrepo_name=`basename $(pwd)`
	fi
	gc=.git/config
	safe_gc=$gc.$$
	if grep "url = https://:" $gc > /dev/null 2>&1; then
                gc_replacement=`sed -e "s;$dp;$HOME;" <<< $(full_path $gc)`
                if [ ! -f "$gc_replacement" ]; then
                        gc_replacement=$gc.bak
                        if [ ! -f $gc_replacement ]; then
                                echo "FAIL $gc corruption seen: url = https://:, could not find a replacement in HOME" 1>&2
                                exit 1
                        fi
		fi
		cmd="cp $gc_replacement $gc"
		echo "$cmd" 1>&2
                if $cmd; then
			echo "WARN $gc corruption seen: url = https://:, replacing: $cmd" 1>&2
		else
                        echo "FAIL $cmd, trying to correct corruption in $gc" 1>&2
                        exit 1
                fi
	fi
        if [ ! -f $gc.bak ]; then
                cp $gc $gc.bak  #       to help the search for an uncorrupt replacement above
        fi
	if grep "url = https://.*sproul:" $gc > /dev/null 2>&1; then
		# left over from a previous run, but we don't want the gc value to sit here indefinitely
		if [ -n "$verbose_mode" ]; then
			echo "Prep_auth sees https"
			cat $gc
			echo EOF
		fi
		cat  $gc | sed -e "s;^\turl = https://\\(.*sproul\\):.*;\turl = git@github.com:\\1/$xrepo_name.git;" > $safe_gc
		echo $safe_gc >> $end_of_process_safe_gc_list
		#t rap "Restore_clean_configs" EXIT
	elif grep "url = git@github.com:.*sproul/" $gc > $t 2>&1; then
		cp $gc $safe_gc
	fi
	# e.g., url = git@github.com:sproul/bin.git
	gh_user=`grep 'url = ' $gc | sed -e 's;.*github.com[:/];;' -e 's;/.*;;'`

	email=`grep 'email =' $HOME/.gitconfig | sed -e 's/.* = //'`
	if [ -f $HOME/.gh.$email ]; then
		hgh=$HOME/.gh.$email
	elif [ -f $HOME/.gh ]; then
		hgh=$HOME/.gh
	else
		hgh=''
	fi
	if [ -n "$verbose_mode" ]; then
		echo "Prep_auth saw ssh"
		cat $gc
		echo EOF
	fi
	if grep 'url = .*@github.com[:/]sproul' $gc > /dev/null 2>&1; then
		# sproul/github -> use gh...
		gh_user=`grep 'url = ' $gc | sed -e 's;.*github.com[:/];;' -e 's;/.*;;'`
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
			#t rap "Restore_clean_configs" EXIT
			cat $safe_gc | sed -e "s;^\turl =.*;\turl = https://$gh_user:`cat $hgh`@github.com/$gh_user/$xrepo_name.git;" > $gc
		fi
	fi
	if [ -f $safe_gc ]; then
		if [ -n "$verbose_mode" ]; then
			echo diff $safe_gc $gc
			diff $safe_gc $gc
		fi
	fi
}
