#!/bin/bash

export files_name=$(echo $two | awk -F. '{printf$1}')
export file_extension=$(echo $two | awk -F. '{printf$2}')

if [[ ($one =~ [^A-Za-z]) || ($one =~ [0-9]) ]];
then
    echo "Wrong. Folder name must contain English letters."
    exit 1
elif [[ (${#one} -gt 7) ]];
then
    echo "No more than 7 letters of the English alphabet in the folder name."
    exit 1
fi

if [[ (${#files_name} > 7) || (${#files_name} < 1) ]];
then
  echo "Wrong number of letters in file name."
  exit 0;
else
  if [[ $files_name =~ [^A-Za-z] ]];
  then
    echo "Wrong. File name must contain English letters."
    exit 0
  fi
fi

if [[ ${#file_extension} > 3 || $file_extension == "" ]];
then
  echo "Wrong number of letters in file extension."
  exit 1
else
  if [[ $file_extension =~ [^A-Za-z] ]];
  then
    echo "Wrong. File extension must contain English letters."
    exit 1
  fi
fi

if [[ !($three =~ [^0-9]) ]]
then
  if [[ ($three -gt 100) || ($three -le 0) ]];
  then
    echo "Invalid size of files."
    exit 1
  fi
else
  echo "The size of files must be an integer."
  exit 1
fi

./run.sh
./time.sh
