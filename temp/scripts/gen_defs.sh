#!/bin/bash

METADATA_DIR=metadata

function increment_release_number()
{
    filepath="$METADATA_DIR/release_number"
    number=`cat $filepath`
    echo $((number + 1)) > $filepath
}

function version_number()
{
    filepath="$METADATA_DIR/version"
    major_minor_path=`cat $filepath`

    filepath="$METADATA_DIR/release_number"
    release_number=`cat $filepath`

    version="$major_minor_path-r$release_number"
    echo "#define VERSION \"$version\""
}

function name()
{
    filepath="$METADATA_DIR/name"
    name=`cat $filepath`

    echo "#define NAME \"$name2\""
}

increment_release_number
echo "#ifndef CP_TOOLS_DEFS_H" > include/defs.h
echo "#define CP_TOOLS_DEFS_H" >> include/defs.h
echo >> include/defs.h

name >> include/defs.h
version_number >> include/defs.h

echo >> include/defs.h
echo "#define CP_TOOLS_TRUE  1" >> include/defs.h
echo "#define CP_TOOLS_FALSE 0" >> include/defs.h

echo >> include/defs.h
echo "#endif" >> include/defs.h
