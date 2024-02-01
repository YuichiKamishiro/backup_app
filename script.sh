#!/bin/bash
####################################
#
# Backup to NFS mount script.
#
####################################
    
while getopts c:w:n: option
do 
    case "${option}"
        in
        w)where=${OPTARG};;
        n)name=${OPTARG};;

    esac
done
archive_file="$name.tar.zst"
# Backup the files using tar.
sudo tar --use-compress-program 'pzstd -19' -cf $where/$archive_file --exclude=/media --exclude=/dev --exclude=/run --exclude=/proc --exclude=/sys --exclude=/tmp --exclude=/lost+found --exclude=/root/.cache --exclude=/home/$USER/.cache --exclude=/home/$USER/.local/share/Trash --exclude=/home/$USER/rosa  --exclude=/var/lib/libvirt --exclude=/var/lib/samba /
