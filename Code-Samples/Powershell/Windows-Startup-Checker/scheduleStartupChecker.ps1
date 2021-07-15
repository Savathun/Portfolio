# Class: CSCI 5742 Cybersecurity Programming
# Name: Daniel Schlatter & Angela Mcneese
# Date: 03/06/2021
# Description: Schedule a task for the StartChecker.ps1 script every five minutes. 

$action = New-ScheduledTaskAction -Execute "Powershell" -Argument "C:\scripts\StartChecker.ps1 5 >> C:\scripts\Startup.log"
$trigger = New-ScheduledTaskTrigger -Daily -At 12AM
$taskName = "CheckStartupFiles"

$task = Register-ScheduledTask -TaskName "CheckStartupFiles" -Trigger $trigger -Action $action
$task.Triggers.Repetition.Duration = "P1D" #Repeat for a duration of one day
$task.Triggers.Repetition.Interval = "PT5M" #Repeat every 30 minutes, use PT1H for every hour
$task | Set-ScheduledTask
