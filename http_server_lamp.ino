#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

ESP8266WiFiMulti WiFiMulti;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String BASE_URL = "http://node-server.fly.dev";
float lightValue = 0;
int DEFAULT_LIMIT = 421;
StaticJsonDocument<512> config;
int timesLooped = 0;

void setup() {
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  Serial.print('\n\n\n');

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Vastriku 2.4", "V8dormit");

  while ((WiFiMulti.run() != WL_CONNECTED)) {
    delay(500);
    Serial.print("Connecting to Wi-Fi...");
  }

  timeClient.begin();
  timeClient.setTimeOffset(3600);

  NTPClient timeClient(ntpUDP, "pool.ntp.org");

  setupConfig();
}

void loop() {

  if (0 % 10 == 0) requestConfig();

  lightValue = analogRead(A0);
  String currentTime = String(timeClient.getHours());
  Serial.println(String(lightValue) + " current hour: " + currentTime);

  Serial.println(String(config[currentTime]));
  if (config[currentTime] <= lightValue && config[currentTime] != 0) {
    digitalWrite(16, LOW);
    Serial.println("LED is OFF");
  } else {
    digitalWrite(16, HIGH);
    Serial.println("LED is ON");
  }

  request(lightValue);
  
  delay(10000);
  timesLooped++;
}

void request(float valueToSend) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;
    String address= BASE_URL + "/status?value=" + String(valueToSend);
      
    if (http.begin(client, address)) {
      Serial.print("[HTTP] GET " + address + "\n");
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println("Response: " + ((payload.length() == 0) ? "<empty>" : payload));
        }
      } else Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());

      http.end();
    } else Serial.printf("[HTTP} Unable to connect\n");
  } else Serial.println("[HTTP] Could not establish connection - no Wi-Fi");
}

void requestConfig() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;
    String address= BASE_URL + "/limits";
      
    if (http.begin(client, address)) {
      Serial.print("[HTTP] GET CONFIG " + address + "\n");
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          deserializeJson(config, payload);
          Serial.println("Config updated - " + payload);
        }
      } else Serial.printf("[HTTP] GET CONFIG... failed, error: %s\n", http.errorToString(httpCode).c_str());

      http.end();
    } else Serial.printf("[HTTP} Unable to connect\n");
  } else Serial.println("[HTTP] Could not establish connection - no Wi-Fi");
}

void setupConfig() {
  for (int i = 0; i < 24; i++) {
    config[String(i)] = 0;
  }
}