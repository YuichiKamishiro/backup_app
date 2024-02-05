#!/bin/bash

sudo rm -rf /usr/share/backup_app && mkdir /usr/share/backup_app && cp -r config /usr/share/backup_app && mkdir /usr/share/backup_app/saves && mv copy.sh /usr/share/backup_app/ && mv recovery.sh /usr/share/backup_app/
