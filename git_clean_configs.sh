#!/bin/bash
set -o pipefail
cd $dp/git
t=/tmp/z
rm */.git/config.*
grep sproul: */.git/config > $t 2> /dev/null
if [ -s $t ]; then
        echo "FAIL unexpectedly found non-empty file $t" 1>&2
        cat $t
        exit 1
fi

exit