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
        c)copy=${OPTARG};;
        w)where=${OPTARG};;
        n)name=${OPTARG};;

    esac
done
archive_file="$name.tar"
# Backup the files using tar.
tar czf $where/$archive_file $copy
