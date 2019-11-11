

rm -R "./_build\*"

mkdir _build
cd _build
cmake .. -DEXPRESSCPP_BUILD_EXAMPLES=ON -DEXPRESSCPP_BUILD_TESTS=ON

cmake --build . -j

