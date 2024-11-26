#include <ArduinoWebsockets.h>
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include "FastLED.h"
#include <ESP32Servo.h>


#define servoPin 4
#define LEDPin 7

using namespace websockets;

String ssid = "Etienne";
String password = "etienne123";
String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
String wsServer = "ws://192.168.176.124:3000";

WebsocketsClient wsClient;
Servo doorServo;

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupPins() {
  pinMode(LEDPin, OUTPUT);
  doorServo.setPeriodHertz(50);
  doorServo.attach(servoPin);
}

void handleServo(bool on) {
  doorServo.write(5);
  delay(2000);
  doorServo.write(150);
}

void onWebSocketMessage(WebsocketsMessage message) {
  Serial.println("Message received: ");
  Serial.println(message.data());

  JSONVar json = JSON.parse(message.data());
  if (JSON.typeof(json) == "undefined") {
    Serial.println("Invalid JSON received");
    return;
  }

  if (json["deviceId"] == deviceID) {
    int pose = (int)json["pose"];
    String event = (const char*)json["event"];

    Serial.print("Pose: ");
    Serial.println(pose);

    if (pose >= 0 && pose < 7) handleServo(true);
    if (event == "cancel") handleServo(false);
  } else {
    Serial.println("Message not intended for this device.");
  }
}

void connectWebSocket() {
  Serial.println("Connecting to WebSocket server...");
  wsClient.onMessage(onWebSocketMessage);
  wsClient.connect(wsServer);

  if (wsClient.available()) {
    Serial.println("WebSocket connected!");
  } else {
    Serial.println("WebSocket connection failed.");
  }
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setupWifi();
  connectWebSocket();
}

void loop() {
  if (!wsClient.available()) {
    Serial.println("WebSocket disconnected. Reconnecting...");
    connectWebSocket();
  }
  wsClient.poll();
}
