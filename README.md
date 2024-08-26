![Ghostkey](https://github.com/raf181/Ghostkey/blob/main/wiki/source/Untitled.webp)

---
# Ghostkey
Ghostykey is a fake HID inputdevice that can be used to simulate keyboard input. It was inspired by the [Rubber Ducky](https://shop.hak5.org/products/usb-rubber-ducky-deluxe) and the [Bash Bunny](https://shop.hak5.org/products/bash-bunny). For hardware, it uses the XIAO SAMD21 board to do the HID through USB, and the XIAO ESP32C6 board to do the WiFi connection and deploy selected payloads.

---

> [!warning] 
> **Warning** these is only a proof of concept.
>
> The project is still in development and is not ready for real use. The project is not responsible for any damage caused by the use of this tool. Use it at your own risk.

# Payloads
### File Retriever:
This PowerShell script searches for files within a specified directory that match given patterns (e.g., files with `.md` extension or containing `*pass*` in their names) and transfers them to a remote server using SCP (Secure Copy Protocol).

### Revers Shell:
Simple reverse shell using Netcat and a simple PowerShell script to create a connection between the target and the attacker.
### DNS Compromiser:
These uses powershell to change the DNS server of the target to a malicious one, allowing the attacker to redirect the target to a fake website or log the target's traffic.

# Collaboration
If you want to collaborate with the project or make your own version of the Ghostkey, feel free to do so. I only ask that you share with me your version of the project so I can learn from it and find ways to improve the Ghostkey.

The project is open source and is under the [GPL-3.0 license](https://github.com/raf181/Ghostkey/blob/main/LICENSE), and I have no intention of changing that. Since it has the following conditions:

| Permissions                                                                                | Limitations               | Conditions                                                                                   |
| ------------------------------------------------------------------------------------------ | ------------------------- | -------------------------------------------------------------------------------------------- |
| Commercial use ✔️<br>Modification ✔️<br>Distribution ✔️<br>Patent use ✔️<br>Private use ✔️ | Liability ❌<br>Warranty ❌ | License and copyright notice ℹ️<br>State changes ℹ️<br>Disclose source ℹ️<br>Same license ℹ️ |


