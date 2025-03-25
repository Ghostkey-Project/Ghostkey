# Function to read API details and encryption password from 'cred' file
function GetAPICredentials {
    $credFilePath = "C:\CARGO\cred"
    
    # Check if 'cred' file exists
    if (Test-Path $credFilePath) {
        try {
            # Read API details and encryption password from 'cred' file
            $credData = Get-Content -Path $credFilePath | ConvertFrom-Json
            return @{
                URL = $credData.URL
                Port = $credData.Port
                esp_id = $credData.esp_id
                delivery_key = $credData.delivery_key
                encryption_password = $credData.encryption_password
            }
        } catch {
            Write-Host "Failed to read 'cred' file: $_"
            return $null
        }
    } else {
        Write-Host "'cred' file does not exist."
        return $null
    }
}

# Function to compress and encrypt folder using 7zr.exe
function CompressAndEncryptFolder {
    param (
        [string]$sourceFolder,
        [string]$outputArchive,
        [string]$encryptionPassword
    )
    
    # Path to 7zr.exe (adjust as necessary)
    $7zrExe = "7zr.exe"

    # Compress and encrypt
    $command = "& `"$7zrExe`" a -t7z `"$outputArchive`" `"$sourceFolder`" -p$encryptionPassword"
    Invoke-Expression $command
}

# Main script

# Ensure 'C:\CARGO' directory exists
$targetDirectory = "C:\CARGO"
if (-not (Test-Path $targetDirectory)) {
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
}

# Define source folder to compress and encrypt
$sourceFolder = "C:\CARGO"
# Define output archive path
$outputArchive = "C:\CARGO.7z"
# Get API credentials and encryption password from 'cred' file
$apiCredentials = GetAPICredentials

if ($apiCredentials) {
    try {
        # Compress and encrypt folder
        CompressAndEncryptFolder -sourceFolder $sourceFolder -outputArchive $outputArchive -encryptionPassword $apiCredentials.encryption_password

        # Machine details
        $machineName = $env:COMPUTERNAME
        $userName = $env:USERNAME
        $date = Get-Date -Format "yyyy-MM-dd HH:mm:ss"

        # Construct API URL
        $apiUrl = "https://$($apiCredentials.URL):$($apiCredentials.Port)/upload"

        # Prepare API payload
        $apiPayload = @{
            MachineName = $machineName
            UserName = $userName
            Date = $date
            File = Get-Content -Path $outputArchive -Encoding Byte
        }

        # Send request to API
        Invoke-RestMethod -Uri $apiUrl -Method Post -ContentType "multipart/form-data" -Body $apiPayload

        Write-Host "Upload to API successful."
    } catch {
        Write-Host "Failed to upload to API: $_"
    }
} else {
    Write-Host "Failed to get API credentials."
}
