#!/bin/bash
cp_mode=''
while [ -n "$1" ]; do
        case "$1" in
                -cp)
                        cp_mode=yes
                ;;
                *)
                        break
                ;;
        esac
        shift
done

src=$1
work=$2
cd $work
find . -type f | sed -e '/\/target\//d' -e '/\.bak$/d' -e '/\/midnight\./d' -e '/\.orig$/d' -e '/LS$/d' -e '/\/rvw$/d' -e '/\/LS\./d' -e '/__dav_content_idcplg/d' -e '/\/LS.f$/d'  -e '/\/missing_from\./d' -e '/\/LS$/d' -e '/eclipse_workspace/d' | sed -e 's;\./;;' |
while read fn; do
        if [ -f "$src/$fn" ]; then
                if diff -b "$work/$fn" "$src/$fn" > /dev/null 2>&1; then
                        continue
                fi
        fi
        if [ -n "$cp_mode" ]; then
                cp.mkdirp "$work/$fn" "$src/$fn"
        fi
done

exit
src="$HOME/git/public-maven-repo"
work="$mrc"
$dp/bin/git.significant_files_in_copy.ls -cp $HOME/git/public-maven-repo $mrc
