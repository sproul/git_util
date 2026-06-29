export ghfn=.git/hooks/pre-commit
export gpc=git.pre_checkin_test.sh

Disable_commit_hook()
{
        if [ ! -f "$gpc" ]; then
                echo "FAIL: expected file at \"$gpc\"" 1>&2
                return 1
        fi
        if [ ! -f "$ghfn" ]; then
                echo "FAIL: expected file at \"$ghfn\"" 1>&2
                return 1
        fi
        if diff $gpc $ghfn > $t; then
                echo "OK diff $gpc $ghfn" 1>&2
                # Actually disable the hook so the commit skips it; Restore_commit_hook
                # re-seeds it from $gpc afterward. (Previously this only verified the
                # two files were identical and never removed the hook, so fast mode
                # never bypassed anything.)
                if rm -f "$ghfn"; then
                        echo "OK disabled commit hook ($ghfn)" 1>&2
                else
                        echo "FAIL could not remove $ghfn to disable hook" 1>&2
                        return 1
                fi
        else
                echo "FAIL diff $gpc $ghfn # they need to be identical so I can re-seed the hook after we are done" 1>&2
                cat $t
                return 1
        fi
        if mv $ghfn /tmp; then
                echo "OK mv $ghfn /tmp" 1>&2
        else
                echo "FAIL mv $ghfn /tmp" 1>&2
                exit 1
        fi
}

Restore_commit_hook()
{
        if [ ! -f "$gpc" ]; then
                echo "FAIL: Restore_commit_hook expected file at \"$gpc\"" 1>&2
                return 1
        fi
        if cp -p $gpc $ghfn; then
                echo "OK cp $gpc $ghfn" 1>&2
        else
                echo "FAIL cp $gpc $ghfn" 1>&2
                return 1
        fi
}
