#!/bin/bash
> filelog
folder_name=$three
last_folder_name=${three: -1}
ffile_name=$file_name
old_file_name=$ffile_name
last_file_name=${ffile_name: -1}
log_date="$(date +"%d%m%y")"
new_date="DATE = $(date +"%d.%m.%y")"

if [[ ${#folder_name} -lt 4 ]]; then
    for (( i=${#folder_name}; i<4; i++ )); do
        folder_name+="$(echo $last_folder_name)"
    done
fi

for (( i=1; i<=$two; i++ )); do
    sudo mkdir "$one/"$folder_name"_"$log_date""
    for (( j=1; j<=$four; j++)); do
    
        sudo fallocate -l $six ""$one"/"$folder_name"_"$log_date"/"$ffile_name"."$file_extension"_"$log_date""
        echo ""$new_date" | "$one"/"$folder_name"_"$log_date"/"$ffile_name"."$file_extension"_"$log_date" | Size of file = $six Kb.">>filelog
        ffile_name+="$(echo $last_file_name)"
    done
    ffile_name=$old_file_name
    folder_name+="$(echo $last_folder_name)"
done
