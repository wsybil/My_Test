#!/bin/bash

export count_folder_name=${#three}
export file_name=$(echo $five | awk -F. '{printf$1}')
export file_extension=$(echo $five | awk -F. '{printf$2}')

if [[ !(-d $one) ]];
then
  echo "Wrong path."
  exit 1
fi

if [[ $two =~ [^0-9] ]];
then
  echo "The number of folders must be an integer."
  exit 1
else
  if [[ $two -gt 100 || $two -le 0 ]];
  then
    echo "Invalid number of folders."
    exit 1
  fi
fi

if [[ ($count_folder_name -gt 7) ]];
then
  echo "Wrong number of letters in folder name."
  exit 1
else
  if [[ $three =~ [^A-Za-z] ]];
  then
    echo "Wrong. Folder name must contain English letters."
    exit 1
  fi
fi

if [[ $four =~ [^0-9] ]];
then
  echo "The number of files must be an integer."
  exit 1
else
  if [[ $four -gt 100 || $four -le 0 ]];
  then
    echo "Files cannot be more than 100."
    exit 1
  fi
fi

if [[ (${#file_name} > 7) || (${#file_name} < 1) ]];
then
  echo "Wrong number of letters in file name."
  exit 0;
else
  if [[ $file_name =~ [^A-Za-z] ]];
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

if [[ !($six =~ [^0-9]) ]]
then
  if [[ ($six -gt 100) || ($six -le 0) ]];
  then
    echo "Invalid size of files."
    exit 1
  fi
else
  echo "The size of files must be an integer."
  exit 1
fi

./run.sh
