
# Wi-Fi SSID and Password Stealer Script
## Overview
This script gathers saved Wi-Fi network names (SSIDs) and passwords from a Windows machine and transfers the information to a remote server using scp. It creates a folder to store the retrieved data, writes the data to a text file, and then securely transfers the folder to the specified remote server. After the transfer, it cleans up by deleting the created folder and its contents.

## Requirements
`PowerShell` installed on the local machine.
`scp` client installed and available in the system's PATH.
`SSH` access to the remote server with appropriate permissions.
`Passwordless SSH authentication` (e.g., using SSH keys) set up between the local machine and the remote server.
# Script Details
## Variables
- $folder: The local directory to store the retrieved Wi-Fi information.
- $remoteServer: The address of the remote server where the data will be sent.
- $remoteUser: The username for the remote server.
- $remotePath: The directory on the remote server where the folder will be saved.
## Steps
1. Create Directory: A directory ($folder) is created to hold the Wi-Fi information.
2. Retrieve Wi-Fi Profiles: The script retrieves a list of saved Wi-Fi network profiles using the netsh wlan show profiles command.
3. Extract SSIDs and Passwords: For each Wi-Fi profile, it extracts the SSID and the password (if available) using the netsh wlan show profile name=$network key=clear command.
4. Write to File: The extracted SSIDs and passwords are written to a text file (wifi.txt) in the created directory.
5. Transfer via SCP: The directory containing the text file is transferred to the remote server using the scp command.
6. Cleanup: The script deletes the created directory and its contents to remove any traces of the operation.