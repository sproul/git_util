:
t=`mktemp`; trap "rm $t" EXIT
git status . > $t
git.changed_files.ls -status_output_fn $t
git.added_files.ls   -status_output_fn $t
