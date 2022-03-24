#!/bin/bash

chmod +x ./error_checking.sh
chmod +x ./run.sh

if [[ $# == 6 ]];
then
  export one=$1
  export two=$2
  export three=$3
  export four=$4
  export five=$5
  export six=$(echo $6 | awk -F"Kb" '{print $1}')

  ./error_checking.sh
  
else
  echo "The script is run with 6 parameters."
fi
