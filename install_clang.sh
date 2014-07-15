#!/bin/bash

set -o errexit
set -o nounset

branch=${1:-"release_34"}
git_location=${2:-"$HOME/llvm.git"}
install_location=${2:-"$HOME/llvm.install"}

# make the path absolute
git_location=$(readlink -f ${git_location})
install_location=$(readlink -f ${install_location})

# some more locations
build_location="${git_location}/build"

# save typing
llvm_org="http://llvm.org/git"

git clone ${llvm_org}/llvm.git ${git_location} --branch $branch
git clone ${llvm_org}/clang.git ${git_location}/tools/clang --branch $branch
git clone ${llvm_org}/clang-tools-extra.git ${git_location}/tools/clang/tools/extra --branch $branch

mkdir -p ${build_location} && cd ${build_location}

${git_location}/configure \
    --prefix=${install_location} \
    --enable-optimized \
    --disable-assertions \
    --enable-targets=host-only

make -j`getconf _NPROCESSORS_ONLN` all check-all install

echo "install done. take care that your shell finds the binaries under '${install_location}'"
