:
. modified_repos.inc

Test()
{
        modified_repos_string=`git_util__modified_repos_Ls`
        if [ -n "$modified_repos_string" ]; then
                echo FAIL modified_repos.sh test requires no modified repos, but saw $modified_repos_string
        else
                echo OK modified_repos.sh test reqt
        fi
}

dry_mode=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                -add)
                        shift
                        repo=$1
                        git_util__modified_repos__Add_repo_to_modified_list "$repo"
                        exit
                ;;
                -dry)
                        dry_mode=-dry
                ;;
                -ls)
                        cat $modified_proj_statuses_string_fn | tr '\n' ' '
                ;;
                -q|-quiet)
                        verbose_mode=''
                ;;
                -refresh)
                        Check_all_repos
                ;;
                -test)
                        Test
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

exit
bx $dp/git_util/modified_repos.sh -refresh
bx $dp/git_util/modified_repos.sh -add $dp/emacs/lisp/n-file.el
exit
bx $dp/git_util/modified_repos.sh -test