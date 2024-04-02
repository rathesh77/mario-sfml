#!/bin/bash

FILE=$1
if test -f "$FILE"; then
    rm $FILE
    echo 'deleted '$FILE
fi
echo $FILE