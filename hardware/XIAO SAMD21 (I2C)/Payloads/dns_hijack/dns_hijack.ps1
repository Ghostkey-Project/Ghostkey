# Script to change DNS settings for all network adapters

# Define the DNS servers you want to set
$dnsServers = "8.8.8.8", "8.8.4.4"  # Example: Google Public DNS servers

# Get all network adapters
$adapters = Get-NetAdapter | Where-Object { $_.Status -eq 'Up' }

foreach ($adapter in $adapters) {
    $adapterName = $adapter.Name
    Write-Output "Configuring DNS for adapter: $adapterName"

    # Get current DNS settings
    $currentDns = Get-DnsClientServerAddress -InterfaceAlias $adapterName -AddressFamily IPv4

    # If current DNS servers are different, set new DNS servers
    if ($currentDns.ServerAddresses -ne $dnsServers) {
        Set-DnsClientServerAddress -InterfaceAlias $adapterName -ServerAddresses $dnsServers -Confirm:$false
        Write-Output "DNS servers updated for adapter: $adapterName"
    } else {
        Write-Output "DNS servers already set correctly for adapter: $adapterName"
    }
}

Write-Output "DNS configuration completed."
