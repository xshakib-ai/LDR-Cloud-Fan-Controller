#include "secrets.h"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const int LDR_PIN = 34;
const int FAN_PIN = 26;

BlynkTimer timer;

int manualFan = 0;
int automaticMode = 0;
int lightThreshold = 50;
int fanState = 0;

BLYNK_WRITE(V2) {
  manualFan = param.asInt();
}

BLYNK_WRITE(V3) {
  automaticMode = param.asInt();
}

BLYNK_WRITE(V4) {
  lightThreshold = param.asInt();
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2, V3, V4);
}

void sendSensorData() {
  int raw = analogRead(LDR_PIN);
  int lightLevel = map(raw, 0, 4095, 0, 100);

  // In automatic mode, switch the fan on
  // when the light level exceeds the threshold.
  if (automaticMode == 1) {
    fanState = (lightLevel >= lightThreshold) ? 1 : 0;
  } else {
    fanState = manualFan;
  }

  digitalWrite(FAN_PIN, fanState);

  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, lightLevel);
    Blynk.virtualWrite(V1, raw);
    Blynk.virtualWrite(V5, fanState);
  }

  Serial.print("Raw LDR: ");
  Serial.print(raw);
  Serial.print(" | Light: ");
  Serial.print(lightLevel);
  Serial.print("% | Fan: ");
  Serial.println(fanState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  analogReadResolution(12);

  // Wokwi's virtual Wi-Fi connection.
Serial.println("Connecting to Wi-Fi...");
WiFi.begin("Wokwi-GUEST", "", 6);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

Serial.println("\nWi-Fi connected!");

Blynk.config(BLYNK_AUTH_TOKEN);
Blynk.connect();

  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}