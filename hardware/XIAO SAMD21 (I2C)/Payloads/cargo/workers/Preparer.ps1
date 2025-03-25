# Specify the directory to search (replace 'C:\Path\To\Search' with your actual directory)
$sourceDirectory = 'C:\Path\To\Search'

# Specify the directory where copies will be stored (replace 'C:\Path\To\Cargo' with your desired directory)
$cargoDirectory = 'C:\Path\To\Cargo'

# Array of file name patterns to search for
$filePatterns = "*pass*", "*.ps1", "other..."

# Create the cargo directory if it doesn't exist
if (-not (Test-Path -Path $cargoDirectory)) {
    New-Item -ItemType Directory -Path $cargoDirectory | Out-Null
}

# Iterate through each file pattern
foreach ($pattern in $filePatterns) {
    # Search for files matching the current pattern in the source directory
    $files = Get-ChildItem -Path $sourceDirectory -Filter $pattern -File -ErrorAction SilentlyContinue

    if ($files) {
        foreach ($file in $files) {
            # Construct the destination path for the cargo directory
            $destinationPath = Join-Path -Path $cargoDirectory -ChildPath $file.Name

            # Copy the file to the cargo directory
            Copy-Item -Path $file.FullName -Destination $destinationPath -Force
            Write-Output "Copied $($file.FullName) to $($destinationPath)"
        }
    } else {
        Write-Output "No files matching pattern '$pattern' found in $sourceDirectory."
    }
}

Write-Output "Script execution completed."
