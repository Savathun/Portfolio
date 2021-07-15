# This a simple bash script to monitor ports on a local network

All ports in the designated IP range are checked for status, and the nmap output is recorded in a file. 

The schedule script then ensures the main script is run every x minutes, and each subsequent checks the old nmap output against the new to check for changes. 
