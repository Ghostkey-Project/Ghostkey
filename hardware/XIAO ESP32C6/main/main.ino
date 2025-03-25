#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char *ssid = "CASA-PONVI";                 
const char *password = "Famili@_9once_Vivanco$"; 

const int I2C_SLAVE_ADDR = 0x08; 

WiFiServer server(80); 

const char *apiHost = "192.168.0.62";      
const int apiPort = 5000;                  
const String apiEndpoint = "/get_command"; 

unsigned long lastApiCheckTime = 0;           
const unsigned long apiCheckInterval = 30000; 

const String esp_id = "esp32_1";                     
const String esp_secret_key = "your_esp_secret_key"; 

void fetchCommandFromAPI()
{
  WiFiClient client;
  if (client.connect(apiHost, apiPort))
  {
    Serial.println("Connected to API");

    client.print(String("GET ") + apiEndpoint + "?esp_id=" + esp_id + "&esp_secret_key=" + esp_secret_key + " HTTP/1.1\r\n" +
                 "Host: " + apiHost + "\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected() && !client.available())
      delay(1);

    String response = "";
    boolean headersEnd = false;
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      if (line == "\n")
      {
        headersEnd = true;
        break;
      }
    }

    if (headersEnd)
    {
      while (client.available())
      {
        response += client.readStringUntil('\r');
      }
    }
    client.stop();

    Serial.println("Raw JSON response:");
    Serial.println(response);

    StaticJsonDocument<1024> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, response);

    if (error)
    {
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
      return;
    }

    String commandFromAPI = jsonDoc["command"].as<String>();
    commandFromAPI.trim();

    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write((const uint8_t *)commandFromAPI.c_str(), commandFromAPI.length());
    Wire.endTransmission();

    Serial.println("Sent command from API: " + commandFromAPI);
  }
  else
  {
    Serial.println("Failed to connect to API");

    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
  }
}

void setup()
{
  Wire.begin(); 
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  connectToWiFi();
  server.begin(); 
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastApiCheckTime >= apiCheckInterval)
  {
    lastApiCheckTime = currentMillis;
    fetchCommandFromAPI();
  }

  handleDirectMode(); 
}

void handleDirectMode()
{
  WiFiClient client = server.accept(); 

  if (client)
  {
    Serial.println("New client connected");

    String currentLine = ""; 
    String command = "";     

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            if (command.length() > 0)
            {
              if (isValidCommand(command))
              {
                Wire.beginTransmission(I2C_SLAVE_ADDR);
                Wire.write((const uint8_t *)command.c_str(), command.length());
                Wire.endTransmission();

                Serial.println("Sent command: " + command);
                client.println("Command received: " + command);
              }
              else
              {
                Serial.println("Invalid command");
                blinkLED(10, 50);
                client.println("Invalid command");
              }
            }
            else
            {
              client.println("Invalid request");
            }

            client.stop();
            Serial.println("Client disconnected");
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }

        if (currentLine.startsWith("GET /command?cmd="))
        {
          int start = currentLine.indexOf('=');
          int end = currentLine.indexOf(' ', start);
          if (start != -1 && end != -1)
          {
            command = currentLine.substring(start + 1, end);
            command.trim();
          }
        }
      }
    }
  }
}

void connectToWiFi()
{
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("HTTP server started");

  blinkLED(2, 250);
  digitalWrite(LED_BUILTIN, LOW);
}

void blinkLED(int numBlinks, int blinkInterval)
{
  for (int i = 0; i < numBlinks; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(blinkInterval);
    digitalWrite(LED_BUILTIN, LOW);
    delay(blinkInterval);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

bool isValidCommand(String command)
{
  return !command.isEmpty();
}