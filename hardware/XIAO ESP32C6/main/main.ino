#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>

// Configuration constants - Move to separate config file in production
const char* const WIFI_SSID = "CASA-PONVI";
const char* const WIFI_PASSWORD = "Famili@_9once_Vivanco$";

// I2C Configuration
constexpr uint8_t I2C_SLAVE_ADDR = 0x08;
constexpr uint32_t I2C_FREQUENCY = 100000; // 100kHz

// Network Configuration
constexpr uint16_t HTTP_SERVER_PORT = 80;
const char* const API_HOST = "192.168.0.62";
constexpr uint16_t API_PORT = 5000;
const char* const API_ENDPOINT = "/get_command";

// Timing Configuration
constexpr unsigned long API_CHECK_INTERVAL = 30000UL; // 30 seconds
constexpr unsigned long WIFI_CONNECT_TIMEOUT = 20000UL; // 20 seconds
constexpr unsigned long HTTP_CLIENT_TIMEOUT = 5000UL; // 5 seconds

// Device Configuration
const char* const ESP_ID = "esp32_1";
const char* const ESP_SECRET_KEY = "your_esp_secret_key";

// LED Configuration
constexpr uint8_t LED_PIN = LED_BUILTIN;
constexpr uint16_t LED_BLINK_FAST = 100;
constexpr uint16_t LED_BLINK_SLOW = 500;

// Global variables
WiFiServer server(HTTP_SERVER_PORT);
unsigned long lastApiCheckTime = 0;
unsigned long lastWiFiCheck = 0;
bool wifiConnected = false; 

// Forward declarations
void connectToWiFi();
void checkWiFiConnection();
bool fetchCommandFromAPI();
void handleDirectMode();
void sendHttpResponse(WiFiClient& client, const String& command);
void blinkLED(uint8_t numBlinks, uint16_t blinkInterval);
bool isValidCommand(const String& command);
bool sendI2CCommand(const String& command);
void handleError(const String& errorMsg, uint8_t ledBlinks = 3);

bool fetchCommandFromAPI() {
  if (!wifiConnected) {
    Serial.println(F("WiFi not connected, skipping API check"));
    return false;
  }

  WiFiClient client;
  client.setTimeout(HTTP_CLIENT_TIMEOUT);
  
  if (!client.connect(API_HOST, API_PORT)) {
    handleError(F("Failed to connect to API"), 5);
    return false;
  }

  Serial.println(F("Connected to API"));

  // Build HTTP request more efficiently
  String request = F("GET ");
  request += API_ENDPOINT;
  request += F("?esp_id=");
  request += ESP_ID;
  request += F("&esp_secret_key=");
  request += ESP_SECRET_KEY;
  request += F(" HTTP/1.1\r\nHost: ");
  request += API_HOST;
  request += F("\r\nConnection: close\r\nUser-Agent: ESP32C6\r\n\r\n");

  client.print(request);

  // Wait for response with timeout
  unsigned long timeout = millis() + HTTP_CLIENT_TIMEOUT;
  while (!client.available() && millis() < timeout) {
    yield(); // Allow other tasks to run
  }

  if (!client.available()) {
    client.stop();
    handleError(F("API response timeout"));
    return false;
  }

  // Skip HTTP headers more efficiently
  bool foundBody = false;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    if (line.length() <= 1) { // Empty line indicates end of headers
      foundBody = true;
      break;
    }
  }

  if (!foundBody) {
    client.stop();
    handleError(F("Invalid HTTP response"));
    return false;
  }

  // Read response body
  String response;
  response.reserve(512); // Pre-allocate memory
  while (client.available()) {
    response += client.readString();
  }
  client.stop();

  if (response.isEmpty()) {
    handleError(F("Empty API response"));
    return false;
  }

  Serial.print(F("API response: "));
  Serial.println(response);

  // Parse JSON response
  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, response);

  if (error) {
    Serial.print(F("JSON parse error: "));
    Serial.println(error.c_str());
    blinkLED(3, LED_BLINK_FAST);
    return false;
  }

  const char* commandFromAPI = jsonDoc["command"];
  if (!commandFromAPI || strlen(commandFromAPI) == 0) {
    Serial.println(F("No command in API response"));
    return false;
  }

  String command = String(commandFromAPI);
  command.trim();

  if (isValidCommand(command)) {
    if (sendI2CCommand(command)) {
      Serial.print(F("Successfully sent API command: "));
      Serial.println(command);
      return true;
    }
  } else {
    Serial.println(F("Invalid command from API"));
    blinkLED(3, LED_BLINK_FAST);
  }

  return false;
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {
    ; // Wait for serial port to connect (max 5 seconds)
  }
  Serial.println(F("\n=== ESP32C6 Battery Monitor Starting ==="));

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize I2C with error checking
  if (!Wire.begin()) {
    handleError(F("I2C initialization failed"));
  } else {
    Wire.setClock(I2C_FREQUENCY);
    Serial.println(F("I2C initialized successfully"));
  }

  // Connect to WiFi
  connectToWiFi();

  // Start HTTP server
  server.begin();
  Serial.println(F("HTTP server started"));
  
  Serial.println(F("=== Setup Complete ===\n"));
}

void loop() {
  const unsigned long currentMillis = millis();

  // Check WiFi connection periodically
  checkWiFiConnection();

  // Fetch command from API at regular intervals
  if (wifiConnected && (currentMillis - lastApiCheckTime >= API_CHECK_INTERVAL)) {
    lastApiCheckTime = currentMillis;
    fetchCommandFromAPI();
  }

  // Handle direct HTTP requests
  handleDirectMode();

  // Yield to allow other tasks to run
  yield();
}

void handleDirectMode() {
  WiFiClient client = server.accept();

  if (!client) {
    return; // No client connected
  }

  Serial.println(F("New client connected"));
  client.setTimeout(HTTP_CLIENT_TIMEOUT);

  String currentLine;
  currentLine.reserve(256); // Pre-allocate memory
  String command;
  command.reserve(64);

  bool requestComplete = false;
  unsigned long clientTimeout = millis() + HTTP_CLIENT_TIMEOUT;

  while (client.connected() && millis() < clientTimeout && !requestComplete) {
    if (client.available()) {
      char c = client.read();
      
      if (c == '\n') {
        if (currentLine.length() == 0) {
          // End of HTTP headers, send response
          sendHttpResponse(client, command);
          requestComplete = true;
        } else {
          // Process the current line
          if (currentLine.startsWith(F("GET /command?cmd="))) {
            int startIdx = currentLine.indexOf('=');
            int endIdx = currentLine.indexOf(' ', startIdx);
            if (startIdx != -1 && endIdx != -1) {
              command = currentLine.substring(startIdx + 1, endIdx);
              command.trim();
              // URL decode basic characters
              command.replace("%20", " ");
              command.replace("%2B", "+");
            }
          }
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
        // Prevent memory overflow
        if (currentLine.length() > 512) {
          break;
        }
      }
    } else {
      yield(); // Allow other tasks to run
    }
  }

  client.stop();
  Serial.println(F("Client disconnected"));
}

void sendHttpResponse(WiFiClient& client, const String& command) {
  // Send HTTP headers
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Access-Control-Allow-Origin: *"));
  client.println(F("Connection: close"));
  client.println();

  // Process command and send response
  if (command.length() > 0) {
    if (isValidCommand(command)) {
      if (sendI2CCommand(command)) {
        Serial.print(F("Direct command sent: "));
        Serial.println(command);
        client.println(F("{\"status\":\"success\",\"message\":\"Command sent successfully\"}"));
      } else {
        client.println(F("{\"status\":\"error\",\"message\":\"Failed to send I2C command\"}"));
      }
    } else {
      Serial.println(F("Invalid direct command"));
      blinkLED(3, LED_BLINK_FAST);
      client.println(F("{\"status\":\"error\",\"message\":\"Invalid command\"}"));
    }
  } else {
    client.println(F("{\"status\":\"error\",\"message\":\"No command provided\"}"));
  }
}

void connectToWiFi() {
  Serial.print(F("Connecting to WiFi: "));
  Serial.println(WIFI_SSID);
  
  // Configure WiFi for better performance
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < WIFI_CONNECT_TIMEOUT) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
    Serial.print(F("."));
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.println(F("WiFi connected successfully!"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Signal strength: "));
    Serial.print(WiFi.RSSI());
    Serial.println(F(" dBm"));
    
    blinkLED(2, LED_BLINK_SLOW);
    digitalWrite(LED_PIN, LOW);
  } else {
    wifiConnected = false;
    handleError(F("WiFi connection failed"), 10);
  }
}

void checkWiFiConnection() {
  const unsigned long currentMillis = millis();
  
  // Check WiFi status every 10 seconds
  if (currentMillis - lastWiFiCheck >= 10000UL) {
    lastWiFiCheck = currentMillis;
    
    if (WiFi.status() != WL_CONNECTED) {
      if (wifiConnected) {
        Serial.println(F("WiFi connection lost, attempting reconnection..."));
        wifiConnected = false;
      }
      connectToWiFi();
    } else if (!wifiConnected) {
      wifiConnected = true;
      Serial.println(F("WiFi reconnected"));
    }
  }
}

void blinkLED(uint8_t numBlinks, uint16_t blinkInterval) {
  for (uint8_t i = 0; i < numBlinks; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(blinkInterval);
    digitalWrite(LED_PIN, LOW);
    delay(blinkInterval);
  }
}

bool isValidCommand(const String& command) {
  if (command.isEmpty() || command.length() > 64) {
    return false;
  }
  
  // Add more validation rules as needed
  // For example, check for allowed characters or specific command formats
  for (size_t i = 0; i < command.length(); i++) {
    char c = command.charAt(i);
    if (!isAlphaNumeric(c) && c != '_' && c != '-' && c != '.' && c != ' ') {
      return false;
    }
  }
  
  return true;
}

bool sendI2CCommand(const String& command) {
  if (command.isEmpty()) {
    return false;
  }
  
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  size_t bytesWritten = Wire.write(reinterpret_cast<const uint8_t*>(command.c_str()), command.length());
  uint8_t result = Wire.endTransmission();
  
  if (result == 0 && bytesWritten == command.length()) {
    Serial.print(F("I2C command sent successfully: "));
    Serial.println(command);
    return true;
  } else {
    Serial.print(F("I2C transmission failed, error code: "));
    Serial.println(result);
    blinkLED(3, LED_BLINK_FAST);
    return false;
  }
}

void handleError(const String& errorMsg, uint8_t ledBlinks) {
  Serial.print(F("ERROR: "));
  Serial.println(errorMsg);
  
  if (ledBlinks > 0) {
    blinkLED(ledBlinks, LED_BLINK_FAST);
  }
}