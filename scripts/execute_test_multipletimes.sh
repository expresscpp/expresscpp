#!/bin/bash

if [ $# -eq 0 ]
then
    echo "No arguments supplied"
    printf "do: \n\t $ execute_test_multipletimes.sh path_to_executable"
    printf "\n"
    exit 1
fi

echo "starting"
echo ""

FAILED=0
for i in `seq 100`; do $1;status=$? ;[ $status -ne 0 ] && FAILED=$((FAILED+1)) ; done

echo ""
echo "failed: $FAILED"
echo "done"


