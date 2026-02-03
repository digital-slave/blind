/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  https://RandomNerdTutorials.com/esp32-web-server-beginners-guide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
// #just edited

#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char *ssid = "Slync_2.4";
const char *password = "9876543210pogo**!!";

// Assign output variables to GPIO pins
const int output6 = 6;
const int output7 = 7;
String output6State = "off";
String output7State = "off";

// Create a web server object
WebServer server(80);
// Function to handle the root URL and show the current states
void handleRoot()
{
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #ec3636; }</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";

  // Display GPIO 6 controls
  html += "<p>GPIO 6 - State " + output6State + "</p>";
  if (output6State == "off")
  {
    html += "<p><a href=\"/6/on\"><button class=\"button\">OFF</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/6/off\"><button class=\"button button2\">UP</button></a></p>";
  }

  // Display GPIO 7 controls
  html += "<p>GPIO 7 - State " + output7State + "</p>";
  if (output7State == "off")
  {
    html += "<p><a href=\"/7/on\"><button class=\"button\">OFF</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/7/off\"><button class=\"button button2\">DOWN</button></a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}
// Function to handle turning GPIO 6 on
void handleGPIO6On()
{
  if (output7State == "on")
  {
    handleGPIO7Off();
  }
  output6State = "on";
  handleRoot();
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
}

// Function to handle turning GPIO 6 off
void handleGPIO6Off()
{
  output6State = "off";
  digitalWrite(output6, LOW);
  Serial.println("LED OFF");
  digitalWrite(LED_BUILTIN, LOW);
  handleRoot();
}

// Function to handle turning GPIO 7 on
void handleGPIO7On()
{
  if (output6State == "on")
  {
    handleGPIO6Off();
  }
  output7State = "on";
  digitalWrite(output7, HIGH);
  Serial.println("LED ON");
  handleRoot();
}

// Function to handle turning GPIO 7 off
void handleGPIO7Off()
{
  output7State = "off";
  digitalWrite(output7, LOW);
  Serial.println("LED OFF");
  handleRoot();
}
IPAddress local_IP(192, 168, 1, 42);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

void setup()
{
  Serial.begin(115200);

  // Initialize the output variables as outputs
  pinMode(output6, OUTPUT);
  pinMode(output7, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output6, LOW);
  digitalWrite(output7, LOW);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server to handle different routes
  server.on("/", handleRoot);
  server.on("/6/on", handleGPIO6On);
  server.on("/6/off", handleGPIO6Off);
  server.on("/7/on", handleGPIO7On);
  server.on("/7/off", handleGPIO7Off);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // Handle incoming client requests
  server.handleClient();
}
