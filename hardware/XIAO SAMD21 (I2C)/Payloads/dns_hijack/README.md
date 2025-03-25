# PowerShell Script for Changing DNS Settings and Network Monitoring

## Overview
This PowerShell script automates the process of changing DNS settings for all network adapters on a Windows machine. It iterates through active network adapters and updates their DNS server addresses to the specified values. Additionally, the script integrates with Pi-hole for DNS-based ad blocking and Wireshark for network traffic monitoring and scanning.

## Requirements
1. **Windows Environment**: The script runs on Windows operating systems with PowerShell installed.
2. **Administrator Privileges**: PowerShell needs to be executed with administrative privileges to modify network settings.
3. **Pi-hole Setup**: Pi-hole should be configured as a DNS server on your network to leverage DNS-based ad blocking.
4. **Wireshark**: Optional but recommended for network traffic monitoring and scanning purposes.

## Script Details

### Changing DNS Settings
The script uses PowerShell cmdlets to:
- Retrieve all active network adapters.
- Update DNS server addresses for each adapter to the specified values (`$dnsServers`).

### Integration with Pi-hole
Pi-hole serves as a DNS sinkhole that blocks unwanted content (ads, trackers) at the DNS level. By setting Pi-hole as the primary DNS server (`$dnsServers`), you enhance network security and privacy.

### Network Monitoring with Wireshark
Wireshark is a powerful tool for capturing and analyzing network packets. You can use Wireshark to:
- Monitor DNS queries and responses to/from Pi-hole.
- Analyze network traffic for potential threats or anomalies.

## Usage Scenarios

### Scenario 1: Setting Up Pi-hole
1. **Install Pi-hole**: Set up Pi-hole on a Raspberry Pi or a dedicated server.
2. **Configure DNS Servers**: Use the script to set Pi-hole's IP address as the primary DNS server (`$dnsServers`).
3. **Verify Configuration**: Check network settings and confirm that Pi-hole is blocking ads effectively.

### Scenario 2: Network Traffic Analysis with Wireshark
1. **Capture Packets**: Use Wireshark to capture packets on the network interface(s) where traffic is of interest.
2. **Filter DNS Traffic**: Filter captured packets to focus on DNS queries and responses.
3. **Analyze Traffic**: Review DNS queries to identify sources of unwanted traffic or potential security issues.

## Security Considerations
- **Secure DNS Servers**: Ensure DNS servers (`$dnsServers`) are reliable and secure.
- **Monitor Traffic**: Regularly monitor network traffic for any anomalies or suspicious activities using Wireshark.
- **Update and Test**: Always test scripts in a controlled environment before deploying in production to avoid unintended consequences.