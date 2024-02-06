#!/bin/bash

sudo rm -rf /usr/share/backup_app && mkdir /usr/share/backup_app && cp -r config /usr/share/backup_app && mkdir /usr/share/backup_app/saves && cp copy.sh /usr/share/backup_app/copy.sh && cp recovery.sh /usr/share/backup_app/recovery.sh
