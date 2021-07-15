#!/bin/bash

# Class: CSCI 5742 Cybersecurity Programming
# Name: Daniel Schlatter & Angela Mcneese
# Date: 03/06/2021
# Description: Schedule a cronjob for the NetMon.sh script every five minutes. 

tmpfile='temp.txt'
crontab -l > $tmpfile
echo '*/5 * * * * $HOME/Documents/NetMon.sh' >> $tmpfile
crontab $tmpfile
rm $tmpfile
