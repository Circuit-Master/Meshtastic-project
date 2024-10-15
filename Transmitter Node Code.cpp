#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <meshtastic.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (!bme.begin()) {
    Serial.println("BME280 initialization failed!");
    while (1);
  }
  Meshtastic.begin();
}

void loop() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure();
  float humidity = bme.readHumidity();
  float dewPoint = temperature - ((100 - humidity) / 5.0);

  String weatherData = "Temp: " + String(temperature) + " C, Pressure: " + String(pressure / 100.0F) + " hPa, Humidity: " + String(humidity) + " %, Dew Point: " + String(dewPoint) + " C";
  Meshtastic.sendText(weatherData);
  delay(150000); // 2.5 minutes in milliseconds
}
