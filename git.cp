#!/bin/bash
# cp git files from dir1 to dir2
dir1="$1"
dir2="$2"

Cp()
{
        cp -pr "$dir1/$1" "$dir2/$1"
}

Cp .git
Cp .htaccess
Cp .openshift
#Cp index.pl
exit
bx $DROP/bin/git.cp $DROP/openshift/teacher $DROP/adyn