#include "secrets.h"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Pin configuration
const int LDR_PIN = 34;
const int FAN_PIN = 26;

// Wokwi Wi-Fi
const char WIFI_SSID[] = "Wokwi-GUEST";
const char WIFI_PASSWORD[] = "";

// Blynk server
const char BLYNK_SERVER[] = "blynk.cloud";
const uint16_t BLYNK_PORT = 80;

BlynkTimer timer;

// Control variables
int manualFan = 0;
int automaticMode = 0;
int lightThreshold = 50;
int fanState = 0;

// V2: Manual Fan Control
BLYNK_WRITE(V2) {
  manualFan = param.asInt();
}

// V3: Automatic Light Mode
BLYNK_WRITE(V3) {
  automaticMode = param.asInt();
}

// V4: Light Threshold
BLYNK_WRITE(V4) {
  lightThreshold = constrain(param.asInt(), 0, 100);
}

// Synchronize dashboard controls
BLYNK_CONNECTED() {
  Serial.println("Blynk connected!");
  Blynk.syncVirtual(V2, V3, V4);
}

// Read LDR and update fan
void sendSensorData() {
  int raw = analogRead(LDR_PIN);

  int lightLevel = map(raw, 0, 4095, 0, 100);
  lightLevel = constrain(lightLevel, 0, 100);

  if (automaticMode == 1) {
    fanState = (lightLevel >= lightThreshold) ? 1 : 0;
  } else {
    fanState = manualFan;
  }

  digitalWrite(FAN_PIN, fanState ? HIGH : LOW);

  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, lightLevel);
    Blynk.virtualWrite(V1, raw);
    Blynk.virtualWrite(V5, fanState);
  }

  Serial.print("Raw LDR: ");
  Serial.print(raw);
  Serial.print(" | Light: ");
  Serial.print(lightLevel);
  Serial.print("% | Threshold: ");
  Serial.print(lightThreshold);
  Serial.print("% | Fan: ");
  Serial.println(fanState ? "ON" : "OFF");
}

// Maintain cloud connection
void checkConnections() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.reconnect();
    return;
  }

  if (!Blynk.connected()) {
    Serial.println("Blynk disconnected. Reconnecting...");
    Blynk.connect(3000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  analogReadResolution(12);

  // Connect to Wokwi Wi-Fi
  Serial.println();
  Serial.println("LDR Cloud Fan Controller");
  Serial.println("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWi-Fi connection failed!");
    Serial.println("Check Wokwi internet access and restart.");
    return;
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure Blynk with an explicit server address
  Blynk.config(BLYNK_AUTH_TOKEN, BLYNK_SERVER, BLYNK_PORT);

  Serial.println("Connecting to Blynk...");

  if (Blynk.connect(10000)) {
    Serial.println("Blynk connection successful!");
  } else {
    Serial.println("Blynk connection failed.");
    Serial.println("Will retry automatically.");
  }

  timer.setInterval(1000L, sendSensorData);
  timer.setInterval(5000L, checkConnections);
}

void loop() {
  Blynk.run();
  timer.run();
}
