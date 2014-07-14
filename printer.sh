#!/bin/bash

set -o errexit
set -o nounset

#
# simple test
#
# get us the offset and type of each member of the "name_of_type", resident in
# the file given as "name_of_header_file.h"
#

# be nice
usage() {
    echo "usage:\n\t./$(basename $0) name_of_type name_of_header_file.h [additional-clang-flags]"
}

# use some sensible defaults, which work
name_of_type=${1:-"SomeSpace::PackedExample"}
name_of_header_file=${2:-"minimal_header.h"}
additional_clang_flags=${3:-""}

# create a dummy-file where we just create a call to the sizeof-operator
# see http://lists.cs.uiuc.edu/pipermail/cfe-dev/2014-July/037778.html
dummy_compile_unit=`mktemp --tmpdir=. dummy.XXXX.cpp`

# create the minimal dummy-cpp-file
cat << EOF > ${dummy_compile_unit}
// automatically generated, don't...
#include "${name_of_header_file}"

// temporary object to call the sizeof-operator
int tmp = sizeof(${name_of_type});
EOF

# call clang to dump the records. don't create and object-file
clang ${additional_clang_flags} -c ${dummy_compile_unit} -Xclang -fdump-record-layouts -o /dev/null

# and remove the dummy
rm ${dummy_compile_unit}

# thats it
