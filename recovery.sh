#!/bin/bash
while getopts n: option
do 
    case "${option}"
        in
        n)name=${OPTARG};;
    esac
done
tar --use-compress-program=unzstd -xvf $name --directory=/
