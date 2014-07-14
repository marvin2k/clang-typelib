#!/bin/bash

#
# extract list of declared things (structs in the moment) from a given header
#

# be nice
usage() {
    echo "usage:\n\t./$(basename $0) name_of_header_file.h"
}

name_of_header_file=${1:-"minimal_header.h"}

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

# hacked a fix path for my libclang.so -- adopt to your need
LD_LIBRARY_PATH=$HOME/.vim/bundle/YouCompleteMe/third_party/ycmd python2 \
    PCX/parse_source.py ${dummy_compile_unit}

echo "structs declared in ${name_of_header_file}:"

xmlstarlet sel -t -m "//CursorKind.STRUCT_DECL" \
    -o "name: " -v "@spelling" -n \
    ${dummy_compile_unit}.xml

rm ${dummy_compile_unit} ${dummy_compile_unit}.xml
