:
grep -v '^#' $dp/git_util/github.dat | sed -e "s;\$dp;$dp;"
