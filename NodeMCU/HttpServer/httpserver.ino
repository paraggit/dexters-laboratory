#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* ssid = ""; // mention here wifi router SSID
const char* password = ""; //mention here wifi router password

// Initialize the WiFi and web server
ESP8266WebServer server(80);

// OLED display pins
const int sda = D2;
const int scl = D1;

// Initialize the OLED display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(
  U8G2_R0,
  /* clock=*/ scl,
  /* data=*/ sda,
  /* reset=*/ U8X8_PIN_NONE);

// NTP client settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Handle root page request
void handleRoot() {
  String html = "<html><body><h1>Hello, world!</h1><p>The time is: ";
  html += timeClient.getFormattedTime();
  html += "</p></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  // Initialize the serial port and display
  Serial.begin(9600);
  display.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Configure the NTP client
  timeClient.begin();
  timeClient.setTimeOffset(-14400);

  // Handle root page request
  server.on("/", handleRoot);

  // Start the web server
  server.begin();
}

void loop() {
  // Update the NTP client
  timeClient.update();

  // Display the time on the OLED display
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.setCursor(0, 20);
  display.print(timeClient.getFormattedTime());
  display.sendBuffer();

  // Handle incoming client requests
  server.handleClient();
}
