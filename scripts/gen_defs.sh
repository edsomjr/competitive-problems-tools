#!/bin/bash

METADATA_DIR=metadata

function increment_build_number()
{
    filepath="$METADATA_DIR/build_number"
    number=`cat $filepath`
    echo $((number+1)) > $filepath
    cat $filepath
}

increment_build_number
