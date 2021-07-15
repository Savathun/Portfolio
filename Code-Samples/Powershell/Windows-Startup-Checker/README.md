# This powershell script will check windows startup files for changes periodically. 

The script will check various startup folders, files, and registry keys for changes since the last check, and output all the changed files to the Write-Output variable. 
The scheduler script will ensure the scirpt runs every x minutes. 
