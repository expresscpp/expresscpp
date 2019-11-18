#!/bin/sh


#  This script searches all source code files and formats them according to the .clang-format style.

maj_min=5
maj_max=3

base=clang-format-9
format=""

# Redirect output to stderr.
exec 1>&2

# check if clang-format is installed
type "$base" >/dev/null 2>&1 && format="$base"

# if not, check all possible versions
# (i.e. clang-format-<$maj_min-$maj_max>-<0-9>)
if [ -z "$format" ]
then
    for j in `seq $maj_max -1 $maj_min`
    do
        for i in `seq 0 9`
        do
            type "$base-$j.$i" >/dev/null 2>&1 && format="$base-$j.$i" && break
        done
        [ -z "$format" ] || break
    done
fi

asdf="$(which $format)"
echo "$asdf"

# no versions of clang-format are installed
if [ -z "$format" ]
then
    echo "$base is not installed. Cannot format code..."
    exit 1
fi

echo "$format was found, going to format your code..." >&2

find $(dirname "$0")/ \
-not \( -path "*/build/*" -prune \) \
-not \( -path "*/_build/*" -prune \) \
-not \( -path "*/cmake/*" -prune \) \
-not \( -path "*/config/*" -prune \) \
-not \( -path "*/scripts/*" -prune \) \
-not \( -path "*/.vscode/*" -prune \) \
-not \( -path "*/third_party/*" -prune \) \
-not \( -path "*/doc/*" -prune \) \
\( -name *.h -o -name *.hpp -o -name *.c -o -name *.cc -o -name *.cpp -o -name *.proto \) \
| xargs $format -i


echo "done formatting with clang"


cmakeFormat="cmake-format"

asdf="$(which $cmakeFormat)"
echo "$asdf"

if [ -z "$cmakeFormat" ]
then
    echo "$cmakeFormat is not installed. Cannot format cmake files..."
    echo "run: sudo pip3 install cmake-format"
    exit 1
fi

echo "$cmakeFormat was found, going to format your cmake scripts..." >&2

find $(dirname "$0")/ \
-not \( -path "*/build/*" -prune \) \
-not \( -path "*/_build/*" -prune \) \
-not \( -path "*/scripts/*" -prune \) \
-not \( -path "*/.vscode/*" -prune \) \
-not \( -path "*/third_party/*" -prune \) \
-not \( -path "*/cmake-build-debug/*" -prune \) \
-not \( -path '*/cmake/conan.cmake' -prune \) \
-not \( -path '*/cmake/debug.cmake' -prune \) \
\( -name *.cmake -o -name CMakeLists.txt ! -iname "*Find*" \) \
| xargs $cmakeFormat -c cmake-format.yaml -i


echo "done formatting with cmake-format"
