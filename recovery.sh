#!/bin/bash
while getopts c:w:n: option
do 
    case "${option}"
        in
        n)name=${OPTARG};;
    esac
done
tar --use-compress-program=unzstd -xvf $name --directory=/
