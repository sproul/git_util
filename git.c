#!/bin/bash
fn="$1"
if [ ! -f "$fn" ]; then
        if [ -f "$HOME/git/$fn" ]; then
                fn=$HOME/git/$fn
        fi
fi

shift
comment="$*"
dir=`dirname "$fn"`
echo "cd $dir"
cd       $dir
bfn=`basename "$fn"`
echo "git commit -m\"$comment\" \"$bfn\""
git       commit -m"$comment" "$bfn"
exit
$dp/bin/git.c $HOME/git/oci_reports/caprep/util/retriever.rb if RETRIEVER does not exist, use hardcoded data instead
