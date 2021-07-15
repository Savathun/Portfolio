# Class: CSCI 5742 Cybersecurity Programming
# Name: Daniel Schlatter & Angela Mcneese
# Date: 03/06/2021
# Description: Check startup files, registry keys and folders for changes within the last $minutes minutes and output changed files to Write-Output. 

param($minutes)
# $minutes: this file will check the files changed within the last $minutes minutes. 

function Add-RegKeyLastWriteTime {
[CmdletBinding()]
param(
    [Parameter(Mandatory, ParameterSetName="ByKey", Position=0, ValueFromPipeline)]
    # Registry key object returned from Get-ChildItem or Get-Item
    [Microsoft.Win32.RegistryKey] $RegistryKey,
    [Parameter(Mandatory, ParameterSetName="ByPath", Position=0)]
    # Path to a registry key
    [string] $Path
)

 begin {
    # Define the namespace (string array creates nested namespace):
    $Namespace = "HeyScriptingGuy"

    # Make sure type is loaded (this will only get loaded on first run):
    Add-Type @"
        using System;
        using System.Text;
        using System.Runtime.InteropServices;

        $($Namespace | ForEach-Object {
            "namespace $_ {"
        })
            public class advapi32 {
                [DllImport("advapi32.dll", CharSet = CharSet.Auto)]
                public static extern Int32 RegQueryInfoKey(
                    Microsoft.Win32.SafeHandles.SafeRegistryHandle hKey,
                    StringBuilder lpClass,
                    [In, Out] ref UInt32 lpcbClass,
                    UInt32 lpReserved,
                    out UInt32 lpcSubKeys,
                    out UInt32 lpcbMaxSubKeyLen,
                    out UInt32 lpcbMaxClassLen,
                    out UInt32 lpcValues,
                    out UInt32 lpcbMaxValueNameLen,
                    out UInt32 lpcbMaxValueLen,
                    out UInt32 lpcbSecurityDescriptor,
                    out System.Runtime.InteropServices.ComTypes.FILETIME lpftLastWriteTime
                );
            }
        $($Namespace | ForEach-Object { "}" })
"@
   
    # Get a shortcut to the type:   
    $RegTools = ("{0}.advapi32" -f ($Namespace -join ".")) -as [type]
}
 process {
    switch ($PSCmdlet.ParameterSetName) {
        "ByKey" {
            # Already have the key
        }
        "ByPath" {
            # We need a RegistryKey object (Get-Item should return that)
            $Item = Get-Item -Path $Path -ErrorAction Stop
 
            # Make sure this is of type [Microsoft.Win32.RegistryKey]
            if ($Item -isnot [Microsoft.Win32.RegistryKey]) {
                throw "'$Path' is not a path to a registry key!"
            }
            $RegistryKey = $Item
        }
    }
 
    # Initialize variables that will be populated:
    $ClassLength = 255 # Buffer size 
    $ClassName = New-Object System.Text.StringBuilder $ClassLength  # Will hold the class name
    $LastWriteTime = New-Object System.Runtime.InteropServices.ComTypes.FILETIME 
           
    switch ($RegTools::RegQueryInfoKey($RegistryKey.Handle,
        $ClassName,
        [ref] $ClassLength,
        $null,  # Reserved
        [ref] $null, # SubKeyCount
        [ref] $null, # MaxSubKeyNameLength
        [ref] $null, # MaxClassLength
        [ref] $null, # ValueCount
        [ref] $null, # MaxValueNameLength
        [ref] $null, # MaxValueValueLength
        [ref] $null, # SecurityDescriptorSize
        [ref] $LastWriteTime
    )) {
         0 { # Success
            # Convert to DateTime object:
            $UnsignedLow = [System.BitConverter]::ToUInt32([System.BitConverter]::GetBytes($LastWriteTime.dwLowDateTime), 0)
            $UnsignedHigh = [System.BitConverter]::ToUInt32([System.BitConverter]::GetBytes($LastWriteTime.dwHighDateTime), 0)
            # Shift high part so it is most significant 32 bits, then copy low part into 64-bit int:
            $FileTimeInt64 = ([Int64] $UnsignedHigh -shl 32) -bor $UnsignedLow
            # Create datetime object
            $LastWriteTime = [datetime]::FromFileTime($FileTimeInt64)
 
            # Add properties to object and output them to pipeline
            $RegistryKey | Add-Member -NotePropertyMembers @{
                LastWriteTime = $LastWriteTime
                ClassName = $ClassName.ToString()
            } -PassThru -Force
        }
        122  { # ERROR_INSUFFICIENT_BUFFER (0x7a)
            throw "Class name buffer too small"
            # function could be recalled with a larger buffer, but for now, just exit
        }
        default {
            throw "Unknown error encountered (error code $_)"
        }
    }
}
}

$paths ='C:\Windows\Tasks','C:\Windows\System32\Tasks',($env:APPDATA + '\Microsoft\Windows\Start Menu\Programs\Startup'),($env:PROGRAMDATA + '\Microsoft\Windows\Start Menu\Programs\Startup')

$files ='c:\autoexec.bat','c:\config.sys','c:\Windows\winstart.bat','c:\Windows\wininit.ini','c:\Windows\dosstart.bat','c:\Windows\system.ini','c:\Windows\win.ini','c:\Windows\System\autoexec.nt','c:\Windows\System\config.nt'

$regkeys='HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run','HKCU:\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run',('HKCU:\Software\Microsoft\Windows NT\CurrentVersion\Windows\Run'),'HKCU:\Software\Microsoft\Windows\CurrentVersion\RunOnce','HKCU:\Software\Microsoft\Windows\CurrentVersion\RunOnceEx','HKCU:\Software\Microsoft\Windows\CurrentVersion\RunServices','HKCU:\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce','HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run','HKLM:\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run','HKLM:\Software\Microsoft\Windows\CurrentVersion\RunOnce','HKLM:\Software\Microsoft\Windows\CurrentVersion\RunOnceEx','HKLM:\System\CurrentControlSet\Services','HKLM:\Software\Microsoft\Windows\CurrentVersion\RunServices','HKLM:\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce',('HKLM:\SOFTWARE\Microsoft\Active Setup\Installed Components'),('HKLM:\SOFTWARE\Wow6432Node\Microsoft\Active Setup\Installed Components'),('HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Font Drivers'),('HKLM:\Software\Microsoft\Windows NT\CurrentVersion\Drivers32'),('HKLM:\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Drivers32'),'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\SharedTaskScheduler','HKLM:\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Explorer\SharedTaskScheduler','HKLM:\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run','HKCU:\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run','HKCU:\Software\Microsoft\WindowsNT\CurrentVersion\Windows\load',('HKLM:\System\CurrentControlSet\Control\Session Manager\BootExecute'),'HKLM:\SOFTWARE\Microsoft\WindowsNT\CurrentVersion\Winlogon\Userinit'

Write-Output "==============" "Begin Scan" (Get-Date) "Scanning files modified in the last $minutes minutes" "Folders" "==============" 
foreach($path in $paths) {
	if(Test-Path $path){
		Get-ChildItem $path -Recurse | Where-Object LastWriteTime -gt (Get-Date).AddMinutes(-$minutes) | Select-Object FullName, LastWriteTime | Sort LastWriteTime
	}
}
Write-Output "Singular Files" "=============="
foreach($file in $files) {
	if(Test-Path $file){
		dir $file | Where-Object LastWriteTime -gt (Get-Date).AddMinutes(-$minutes) | Select-Object FullName, LastWriteTime
	}
}
Write-Output "Registry Keys" "=============="
foreach($regkey in $regkeys){
	if(Test-Path $regkey){
		Get-ChildItem $regkey | Add-RegKeyLastWriteTime | Where LastWriteTime -gt (Get-Date).AddMinutes(-$minutes) | Select Name, LastWriteTime | Sort LastWriteTime
		Get-Item $regkey | Add-RegKeyLastWriteTime | Where LastWriteTime -gt (Get-Date).AddMinutes(-$minutes) | Select Name, LastWriteTime | Sort LastWriteTime
	}
}
Write-Output "End Scan" "=============="
