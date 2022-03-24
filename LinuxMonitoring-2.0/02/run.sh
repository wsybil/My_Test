#!/bin/bash
> filelog

random_folder_name="$(compgen -d / | shuf -n1)"
folder_name=$one
last_folder_name=${one: -1}
file_name=$files_name
old_filee_name=$file_name
last_file__name=${file_name: -1}

file_date="$(date +"%d%m%y")"
log_date="DATE = $(date +"%d.%m.%y")"

if [[ ${#folder_name} -lt 5 ]];
then
    for (( i=${#folder_name}; i<5; i++ ));
    do
        folder_name+="$(echo $last_folder_name)"
    done
fi

tmp_folder=100
for (( i=1; i<=$tmp_folder; i++ )); do
    random_folder_name="$(compgen -d / | shuf -n1)"
    tmp_file="$(shuf -i 50-100 -n1)"
    if [[ $random_folder_name == "/bin" || $random_folder_name == "/sbin" ||\
        $random_folder_name == "/proc" || $random_folder_name == "/sys" ]];
        then
        tmp_folder+="$(echo $tmp_folder+1)"
        continue
    fi

    sudo mkdir "$random_folder_name/"$folder_name"_"$file_date"" 2>/dev/null
    for (( j=1; j<=${tmp_file}; j++)); do
        size="$(df -h / | awk '{print $4}' | tail -n1)"
        if [[ ${size: -1} == "M" ]]; then
            exit 1
        fi
            sudo fallocate -l $three"M" ""$random_folder_name"/"$folder_name"_"$file_date"/"$file_name"."$file_extension"_"$file_date"" 2>/dev/null
            echo ""$log_date" | "$random_folder_name"/"$folder_name"_"$file_date"/"$file_name"."$file_extension"_"$file_date" | Size of file = $three Mb.">>filelog
            file_name+="$(echo $last_file__name)"
    done
    file_name=$old_filee_name
    folder_name+="$(echo $last_folder_name)"
done
