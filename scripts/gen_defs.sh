#!/bin/bash

METADATA_DIR=metadata

function increment_release_number()
{
    filepath="$METADATA_DIR/release_number"
    number=`cat $filepath`
}

function version_number()
{
    filepath="$METADATA_DIR/version"
    major_minor_path=`cat $filepath`

    filepath="$METADATA_DIR/release_number"
    release_number=`cat $filepath`

    version="$major_minor_path-r$release_number"
    echo "#define VERSION $version"
}

function name()
{
    filepath="$METADATA_DIR/name"
    name=`cat $filepath`

    echo "#define NAME \"$name\""
}

increment_release_number
name > include/defs.h
version_number >> include/defs.h
