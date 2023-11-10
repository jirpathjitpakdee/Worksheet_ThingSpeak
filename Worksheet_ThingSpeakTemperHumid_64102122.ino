#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

unsigned long channelID = 2340079;
const char* writeKey = "YOMW7ZF8RVM3DWCF";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

WiFiClient client;
DHT dht14(D4, DHT11);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("iPhone", "4321zszs");
  ThingSpeak.begin(client); 
  dht14.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin("iPhone", "4321zszs");
        delay(5000);
      }
      Serial.println("Connected.");
    }

    float humid = dht14.readHumidity();
    float temper = dht14.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(humid);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temper);
    Serial.println(" °C");

    ThingSpeak.setField(1, humid);
    ThingSpeak.setField(2, temper);

    int X = ThingSpeak.writeFields(channelID, writeKey);
    if (X == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(X));
    }

    lastTime = millis();
  }
}