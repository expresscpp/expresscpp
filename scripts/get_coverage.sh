#!/bin/bash

echo "starting"
echo ""

GCOV_VERSION="gcov-9"
MINIMAL_COVERAGE=40

mkdir -p coverage || true
rm -rf coverage/*

gcovr -r `pwd` \
    --gcov-executable $GCOV_VERSION \
    --exclude=".*test*." \
    --exclude=".*example*." \
    --exclude=".*/build/*." \
    --exclude=".*/_build/*." \
    --fail-under-line=$MINIMAL_COVERAGE

gcovr -r `pwd` \
    --gcov-executable $GCOV_VERSION \
    --exclude=".*test*." \
    --exclude=".*example*." \
    --exclude=".*/_build/*." \
    --exclude=".*/build/*." \
    -s -p --html --html-details \
    -o coverage/coverage.html

echo ""
echo "done"
