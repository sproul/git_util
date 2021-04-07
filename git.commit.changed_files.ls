#!/bin/bash
commit=$1

if [ -z "$commit" ]; then
        echo "$0: expected a value for \"commit\" but saw nothing" 1>&2
        exit 1
fi

git show --name-only $commit
exit
cd ~nsproul/git/carson; $dp/bin/git.commit.changed_files.ls ce2bf119 > $k