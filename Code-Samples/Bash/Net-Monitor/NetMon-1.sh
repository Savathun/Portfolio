#!/bin/bash

# Class: CSCI 5742 Cybersecurity Programming
# Name: Daniel Schlatter & Angela Mcneese
# Date: 03/06/2021
# Assignment: HW3 Part 2
# Description: Run nmap against IP range 192.168.10.0/24, ports 1-65536 and check if any new ports have been opened/closed since the last scan. Log Changes to PortLog.log.

if [ ! -f Ports.txt ] ; then
	echo "Initial Scan at $(date)" > PortLog.log
	nmap 192.168.10.1-254 -p 1-65535 -oG Ports.txt
else 
	nmap 192.168.10.1-254 -p 1-65535 -oG NewPorts.txt
	old=$(grep "Ports" Ports.txt)
	new=$(grep "Ports" NewPorts.txt)
	if [[ "$old" != "$new" ]] ; then
		echo "Ports changed at $(date)"  >> PortLog.log
		echo "$new" >> PortLog.log
		cat NewPorts.txt > Ports.txt
	else 
		echo "Ports remained the same at $(date)" >> PortLog.txt
	fi
fi
