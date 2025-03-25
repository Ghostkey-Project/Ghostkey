# DNS Hijack Payload
A payload that modifies DNS settings on Windows systems using PowerShell commands.

## Description
This payload changes the DNS server settings for all active network adapters on the target system. It requires administrative privileges to execute successfully.

## Function Usage

```cpp
executeDNSHijack(dns1, dns2)
```

### Parameters
- `dns1` (String): Primary DNS server IP address (default: "8.8.8.8")
- `dns2` (String): Secondary DNS server IP address (default: "1.1.1.1")

### Examples

```cpp
// Use Google DNS servers
executeDNSHijack("8.8.8.8", "8.8.4.4");

// Use Cloudflare DNS servers
executeDNSHijack("1.1.1.1", "1.0.0.1");

// Use custom DNS servers
executeDNSHijack("192.168.1.1", "192.168.1.2");
```

## How It Works
1. Opens PowerShell with administrative privileges
2. Identifies all active network adapters
3. Changes DNS server settings for each adapter
4. Clears DNS cache to apply changes immediately

## PowerShell Commands Used
- `Get-NetAdapter`: Lists network adapters
- `Set-DnsClientServerAddress`: Changes DNS server settings
- `Clear-DnsClientCache`: Clears DNS cache

## Common DNS Servers

| Provider   | Primary DNS  | Secondary DNS |
|------------|-------------|---------------|
| Google     | 8.8.8.8     | 8.8.4.4      |
| Cloudflare | 1.1.1.1     | 1.0.0.1      |
| OpenDNS    | 208.67.222.222 | 208.67.220.220 |