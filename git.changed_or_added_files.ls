:
t=`mktemp`; trap "rm $t" EXIT
git status . --short > $t
git.changed_files.ls -status_output_fn $t
git.added_files.ls   -status_output_fn $t
exit
$dp/git_util/git.changed_or_added_files.ls