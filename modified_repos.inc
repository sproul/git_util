# $HOME/.profile sets PS1 to call git_util__modified_repos_Ls
# alias gu calls git.up to call git_util__modified_repos__Remove_from_list
# alias gu calls git.up to call git_util__modified_repos__Remove_from_list

modified_repo_statuses_string_fn="$dp/.modified_repo_statuses"

git_util__modified_repos__Remove_from_list()
{
        proj=$1
        if [ -s $modified_repo_statuses_string_fn ]; then
                if grep "^$proj$" $modified_repo_statuses_string_fn > /dev/null 2>&1; then
                        grep -v "^$proj$" $modified_repo_statuses_string_fn > /tmp/modified_repo_statuses_string_fn.new
                elif grep "/$proj$" $modified_repo_statuses_string_fn > /dev/null 2>&1; then
                        # works with basename also
                        grep -v "/$proj$" $modified_repo_statuses_string_fn > /tmp/modified_repo_statuses_string_fn.new
                else
                        return
                fi
                mv /tmp/modified_repo_statuses_string_fn.new $modified_repo_statuses_string_fn
        fi
}

git_util__modified_repos__Add_to_list()
{
        proj=$1
        if ! grep "^$proj$" $modified_repo_statuses_string_fn > /dev/null 2>&1; then
                (
                echo "$proj"
                cat $modified_repo_statuses_string_fn
                ) | sort > $modified_repo_statuses_string_fn.new
                mv $modified_repo_statuses_string_fn.new $modified_repo_statuses_string_fn
        fi
}

git_util__modified_repos__Check_repo()
{
        proj_root_dir="$1"
        (
        cd "$proj_root_dir"
        if ! output=`git status --short > $t 2>&1`; then
                echo "$0: output=`git status --short` failed, exiting..." 1>&2
                cat $t
                rm $t
                exit 1
        fi        
        rm $t
        if [ -n "$output" ]; then
                git_util__modified_repos__Add_to_list `basename "$proj_root_dir"`
        fi
        )
}

git_util__modified_repos__Check_all_repos()
{
        for proj in `git.repo.ls`; do
                git_util__modified_repos__Check_repo "$proj"
        done
}

Checkin_all()
{
        echo IMPL
}

git_util__modified_repos_Ls()
{
        touch $modified_repo_statuses_string_fn
        cat   $modified_repo_statuses_string_fn | tr '\n' ' '
}

git_util__modified_repos_Ls()
{
        if [ -n "$modified_repo_statuses_string_fn" ]; then
                cat $modified_repo_statuses_string_fn | sed -e 's;.*/;;' | tr '\n' ' '
        fi
}
