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

Github_https_remote_uses_osxkeychain()
{
        [ "`uname`" = Darwin ] || return 1
        git config --get-all credential.helper | grep -q osxkeychain || return 1
        git remote get-url origin 2>/dev/null | grep -q '^https://github\.com/'
}

Osxkeychain_github_password()
{
        printf 'protocol=https\nhost=github.com\n' | git credential-osxkeychain get | sed -n 's/^password=//p'
}

Store_dot_gh_token_in_osxkeychain()
{
        printf 'protocol=https\nhost=github.com\nusername=%s\npassword=%s\n' "${GITHUB_USER-sproul}" "$1" | git credential-osxkeychain store
        echo "OK stored github.com credential from $HOME/.gh in osxkeychain"
}

Sync_osxkeychain_github_credential_from_dot_gh()
{
        Github_https_remote_uses_osxkeychain || return 0
        if [ ! -s "$HOME/.gh" ]; then
                echo "FAIL $HOME/.gh is missing or empty; it is the source of truth for the github.com credential" 1>&2
                return 1
        fi
        dot_gh_token=`tr -d '\n' < $HOME/.gh`
        if [ "`Osxkeychain_github_password`" != "$dot_gh_token" ]; then
                Store_dot_gh_token_in_osxkeychain "$dot_gh_token"
        fi
}
