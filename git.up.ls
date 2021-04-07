#!/bin/bash
# list new or modified files
git status . | grep / | sed -e '/ and\/or /d' -e 's/.*: *//' -e 's/^\t//' -e '/update what will be committed/d' -e '/Your branch /d' | sort | uniq
