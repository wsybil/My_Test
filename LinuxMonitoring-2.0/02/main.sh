#!/bin/bash

chmod +x ./error_checking.sh
chmod +x ./run.sh
chmod +x ./time.sh

if [[ $# == 3 ]];
then
    export start=$(date +%s%N)
    export time=$(date +%H:%M)
    export one=$1
    export two=$2
    export three=$(echo $3 | awk -F"Mb" '{print $1}')

    ./error_checking.sh

else
    echo "The script is run with 3 parameters."
fi
