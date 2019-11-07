#!/bin/bash

# dependencies:
# apt-get install python3 python3-pip graphviz valgrind kcachegrind
# pip3 install gprof2dot
# you can also use kcachegrind to visualize the results...

if [ $# -eq 0 ]
then
    echo "No arguments supplied"
    printf "do: \n\t $ build_call_graph.sh path_to_executable"
    printf "\n"
    exit 1
fi

echo "starting"
echo ""
# cd _build

valgrind --tool=callgrind $1

echo ""
echo "valgrind'ed"
echo ""

fileName=$(find . -name 'callgrind.out*' -type f -print -quit)
echo "Filename: $fileName"

gprof2dot --format=callgrind --output=out.dot $fileName

echo ""
echo "dot'ed"
echo ""

dot -Tpng out.dot -o graph.png

echo ""
echo "png'ed"
echo ""
echo "done"


