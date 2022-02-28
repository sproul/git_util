:
. modified_repos.inc

Test1()
{
        label="$1"
        actual_output=$t.actual
        git_util__modified_repos_Ls > $actual_output
        cat | assert.f.is "$label" $actual_output
}
Test()
{
        echo "clearing out record of modified repos in order to simplify testing..."
        rm -f $modified_repo_statuses_string_fn
        t=`mktemp`
        trap "rm $t*" EXIT
        
        cat /dev/null | Test1 modified_repos.sh.env_clean
        git_util__modified_repos__Add_to_list "abc"
        echo abc | Test1 modified_repos.sh.added1
        $0 -add xyz
        echo 'abc xyz' | Test1 modified_repos.sh.added2
        git_util__modified_repos__Add_to_list "mno"
        echo 'abc mno xyz' | Test1 modified_repos.added3
        git_util__modified_repos__Remove_from_list "mno"
        echo 'abc xyz' | Test1 modified_repos.sh.rm_middle
        git_util__modified_repos__Remove_from_list "xyz"
        echo 'abc' | Test1 modified_repos.sh.rm_end
        git_util__modified_repos__Remove_from_list "abc"
        cat /dev/null | Test1 modified_repos.sh.env_clean_again
}

dry_mode=''
verbose_mode=''
while [ -n "$1" ]; do
        case "$1" in
                -add)
                        shift
                        repo=$1
                        git_util__modified_repos__Add_to_list "$repo"
                        exit
                ;;
                -dry)
                        dry_mode=-dry
                ;;
                -ls)
                        git_util__modified_repos_Ls
                ;;
                -q|-quiet)
                        verbose_mode=''
                ;;
                -refresh)
                        Check_all_repos
                ;;
                -test)
                        Test
                        exit
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