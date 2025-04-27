# Ghostkey SD Card Edition - Unified Version

This is a consolidated version of Ghostkey that reads and executes instructions from an SD card, supporting both custom format instructions and Ducky Script. This unified version reduces the amount of code stored on the ESP device while maintaining maximum flexibility.

## Hardware Requirements

1. XIAO SAMD21 or compatible ESP board
2. Micro SD card adapter compatible with your board
3. Micro SD card (formatted as FAT16/FAT32)

## Features

- **Dual Script Mode:** Supports both custom instruction format and standard Ducky Script
- **Layout Independent Typing:** Works correctly regardless of keyboard language settings
- **Minimal Footprint:** Optimized to use minimal storage space
- **Visual Feedback:** LED indicators for status and errors
- **Debug Output:** Comprehensive serial monitoring for troubleshooting

## Setup Instructions

1. **Wire the SD card module to your XIAO SAMD21 board:**
   - Connect VCC to 3.3V
   - Connect GND to GND
   - Connect MISO to D9 (pin 10)
   - Connect MOSI to D10 (pin 10)
   - Connect SCK to D8 (pin 8)
   - Connect CS to D7 (pin 7)

2. **Format your SD card:**
   - Format the SD card as FAT16 or FAT32

3. **Create script file:**
   - For custom format: Create a file named `instructions.txt`
   - For Ducky Script: Create a file named `payload.txt`
   - Add commands following the appropriate format
   - Save the file to the root directory of the SD card

4. **Configuration (Optional):**
   - Open `ghostkey_sd.ino` in a text editor
   - Edit the mode settings at the top of the file:
     - `SCRIPT_MODE`: Set to 0 for custom format or 1 for Ducky Script
     - `USE_LAYOUT_INDEPENDENT`: Set to true/false for layout independence

5. **Upload the code:**
   - Open `ghostkey_sd.ino` in the Arduino IDE
   - Select your board type
   - Upload the code to your device

## Script Formats

### Custom Format
Commands in `instructions.txt` should be written one per line in the format: `COMMAND:PARAMETERS`

Example:
```
DELAY:1000
RUN
TYPELINE:notepad
DELAY:500
TYPE:Hello, this is custom format!
```

### Ducky Script Format
Commands in `payload.txt` should follow standard Ducky Script syntax:

Example:
```
REM This is a comment
DELAY 1000
GUI r
STRING notepad
ENTER
STRING Hello, this is Ducky Script!
```

## LED Indicators

- **LED_USER (Orange)** - Flashes at startup and when processing is complete
- **LED_RX (Blue)** - Flashes briefly when processing each instruction
- **LED_TX (Blue)** - Flashes in case of errors
  - 5 rapid flashes: SD card initialization failed
  - 3 slower flashes: Failed to open script file

## Debugging with Serial Monitor

The Ghostkey provides detailed debug information through its serial port. To use this feature:

1. Connect your device to your computer via USB
2. Open the Arduino IDE
3. Select the correct COM port for your device
4. Open the Serial Monitor (Tools > Serial Monitor)
5. Set the baud rate to 9600

The serial output provides information about:

- Initialization process and configuration settings
- SD card mounting status and file system details
- Script file detection and validation
- Line-by-line command execution in real-time
- Error messages and troubleshooting hints
- LED activity and status indicators
- Execution flow and completion status

Example serial output:
```
Ghostkey SD Card Edition - Startup
Script Mode: Ducky Script
Layout Independent Mode: Enabled
Keyboard initialized
Initializing SD card with CS on pin 7...SUCCESS!
SD card initialized.
Looking for primary script file: /payload.txt - FOUND
Using script file: /payload.txt
Opening script file...
File opened successfully
Executing script...
Line 1: REM This is a demo script
Skipping comment: REM This is a demo script
Line 2: DELAY 1000
Ducky command: DELAY, Params: 1000
Line 3: STRING Hello World
Ducky command: STRING, Params: Hello World
Script execution complete
Ghostkey idle - Script execution complete
```

## Troubleshooting

1. **SD card not recognized:**
   - Check wiring connections
   - Try a different SD card
   - Verify the SD card is formatted correctly
   - Check Serial Monitor for detailed error messages

2. **Scripts not executing:**
   - Check that your script file is in the root directory of the SD card
   - Verify the file format matches the specifications
   - Check for syntax errors in your script file
   - Make sure SCRIPT_MODE is set correctly for your file type
   - View Serial Monitor for command execution logs

3. **LED_RX flashing continuously:**
   - This indicates an error reading the SD card
   - Check SD card connections
   - Try reformatting the SD card
   - Serial Monitor will show specific SD card errors

4. **Keyboard layout issues:**
   - If characters are not typing correctly, make sure USE_LAYOUT_INDEPENDENT is set to true
   - For special characters not supported in layout-independent mode, you may need to modify the character mapping

## Extending the Code

To add new commands:

1. Update the `processInstructionLine()` or `processDuckyLine()` function
2. Add any necessary helper functions
3. Update the documentation with the new commands

## Security Considerations

Remember that anyone with access to the SD card can modify the instructions. Consider physical security measures if your device will be used in sensitive environments.
