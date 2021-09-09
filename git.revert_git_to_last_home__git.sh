#!/bin/bash
cwd=`pwd`
project_name=`basename $cwd`

last_home_git=$HOME/git/$project_name/.git
if [ ! -d "$last_home_git" ]; then
        echo "$0: error: could not find directory \"$last_home_git\"" 1>&2
        exit 1
fi

rm -rf /tmp/.git
echo.clean "mv $cwd/.git /tmp/.git"
if ! mv        $cwd/.git /tmp/.git; then
        echo "$0: mv $cwd/.git /tmp/.git failed, exiting..." 1>&2
        exit 1
fi
echo.clean "cp -pr $last_home_git $cwd"
cp             -pr $last_home_git $cwd

exit