. proxy_configure.sh
Changes_seen_in_git_status_output()
{
        git_status_output_fn="$1"
        if grep modified: $git_status_output_fn > /dev/null 2>&1; then
                return 0
        fi
        if grep '^Untracked files:' $git_status_output_fn > /dev/null 2>&1; then
                return 0
        fi
        if grep 'deleted:' $git_status_output_fn > /dev/null 2>&1; then
                return 0
        fi
        return 1
}
