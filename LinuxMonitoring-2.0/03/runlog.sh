#!/bin/bash

file=$(cat ../02/filelog | grep DATE | awk '{print$5}')
count=1
while [[ -z ${file[$count]} ]]
do
  rm -rf $file[$count]
  (( count++ ))
  echo $count
done
