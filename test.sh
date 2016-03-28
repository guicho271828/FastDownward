#!/bin/bash

with-dir (){
    mkdir -p $1
    pushd $1
    shift
    echo $@
    eval $*
    popd
}

# CC=clang-3.5 CXX=clang-3.5

# with-dir builds/debug32/ LINKER=clang-3.5 CC=clang-3.5 CXX=clang-3.5 cmake ../../src
# with-dir builds/debug32/ LINKER=clang-3.5 CC=clang-3.5 CXX=clang-3.5 make

# CC=clang CXX=clang cmake ../../src
# CC=clang CXX=clang make

./cgroup-setup.sh

./build.py &
./build.py --debug &

wait && cgexec -g memory:$(whoami)/ misc/tests/run-all-code-tests
