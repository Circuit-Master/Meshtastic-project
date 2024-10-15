#include <meshtastic.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
AsyncWebServer server(80);
String dataString = "[";
const int DATA_POINTS = 576; // (24 hours * 60 minutes) / 2.5 minutes

void setup() {
  Serial.begin(115200);
  while (!Serial);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Meshtastic.begin();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", dataString + "]");
  });
  
  server.begin();
}

void loop() {
  if (Meshtastic.receive()) {
    String receivedMsg = Meshtastic.message();
    if (receivedMsg.indexOf("WeatherNode") != -1) {
      updateData(receivedMsg);
    }
  }
}

void updateData(String data) {
  StaticJsonDocument<512> doc;
  doc["time"] = millis();
  doc["data"] = data;

  String jsonString;
  serializeJson(doc, jsonString);
  
  if (dataString.length() >= DATA_POINTS * 50) { // Adjust buffer size accordingly
    dataString.remove(0, dataString.indexOf('}') + 1);
  }
  dataString += jsonString + ",";
}
