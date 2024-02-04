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
sudo tar czf $where/$name /etc
