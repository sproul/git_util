:
# list the SHAs associated with FN, oldest first

Test()
{
        git.log.SHA.ls $dp/data/n > $t.actual_SHAs
	cat <<EOF | assert.f.is git.log.SHA.ls $t.actual_SHAs
633e186fd4e6debb59855b1bfdd35ee05921e654
8eb52756c69f11014159a87aee9a8e3ee24f88f6
75cd728572454a96fa5a1a04adea5b3833b7da19
b5416596e94827fe9344e67e20dc3f38fe260acb
81e6ba4980476f8f716b27c9b2fd85ad8b047079
EOF
        Test_hash_value_to_negative_SHA_index_with_newest_version_considered_0 633e186fd4e6debb59855b1bfdd35ee05921e654 0 -4
        Test_hash_value_to_negative_SHA_index_with_newest_version_considered_0 8eb52756c69f11014159a87aee9a8e3ee24f88f6 1 -3
        Test_hash_value_to_negative_SHA_index_with_newest_version_considered_0 b5416596e94827fe9344e67e20dc3f38fe260acb 3 -1
}

Test_hash_value_to_negative_SHA_index_with_newest_version_considered_0()
{
        sha=$1
        index=$2
        expected_negative_SHA_index_with_newest_version_considered_0=$3
        if git.SHA.checkout $sha $dp/data/n; then
                echo "OK git.SHA.checkout $sha $dp/data/n index is $index" 1>&2
	else
                echo "FAIL git.SHA.checkout $sha $dp/data/n index is $index" 1>&2
                exit 1
        fi
        blob_hash=`git.blob_hash.for_file $dp/data/n`
        negative_SHA_index_with_newest_version_considered_0=`git.hash_value_to_negative_SHA_index_with_newest_version_considered_0 $dp/data/n $blob_hash`
        if [ "$negative_SHA_index_with_newest_version_considered_0" = $expected_negative_SHA_index_with_newest_version_considered_0 ]; then
                echo "OK [ negative_SHA_index_with_newest_version_considered_0 (for index==$index) == $negative_SHA_index_with_newest_version_considered_0 ]" 1>&2
        else
                echo "FAIL expected $expected_negative_SHA_index_with_newest_version_considered_0, but saw $negative_SHA_index_with_newest_version_considered_0" 1>&2
                exit 1
        fi
}

debug_mode=''
dry_mode=''
t=`mktemp`; trap "rm $t*" EXIT
verbose_mode=''
while [ -n "$1" ]; do
	case "$1" in
		-dry)
			dry_mode=-dry
		;;
		-q|-quiet)
			verbose_mode=''
		;;
		-test)
			Test
                        exit
		;;
		-v|-verbose)
			verbose_mode=-v
		;;
		-x)
			set -x
			debug_mode=-x
		;;
		*)
			break
		;;
	esac
	shift
done

fn=$1
if [ ! -f "$fn" ]; then
	echo "FAIL: expected file at \"$fn\"" 1>&2
	exit 1
fi
cd `dirname $fn`
if git log --pretty=format:"%H" --reverse -- $fn; then
        echo ''         #       for some reason the last line has no '\n' -- this causes some trouble down the line, so just add the newline char here
	if [ -n "$verbose_mode" ]; then
		echo "OK git log --pretty=format:%H -- $fn" 1>&2
	fi
else
	echo "FAIL git log --pretty=format:%H -- $fn" 1>&2
	exit 1
fi
exit
bx $dp/git/git_util/git.log.SHA.ls $dp/data/nk
exit
bx $dp/git/git_util/git.log.SHA.ls -test