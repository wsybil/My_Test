#!/bin/bash

log=1
date=2
mask=3

if [ -n "$2" ]
then
    echo "Script is used with one parameter."
    exit 1
elif [ "$1" = "$log" ]
then
    chmod +x ./runlog.sh
    ./runlog.sh
elif [ "$1" = "$date" ]
then
    chmod +x ./rundate.sh
    ./rundate.sh
elif [ "$1" = "$mask" ]
then
    chmod +x ./runmask.sh
    ./runmask.sh
else
    echo "neverno ykazan parametr"
fi
