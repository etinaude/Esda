#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <Arduino_JSON.h>
#include "FastLED.h"

using namespace websockets;

// LED and valve pin definitions
#define NumPixels 1
#define LEDPin 11
#define valvePinA 9
#define valvePinB 10

CHSV leds[NumPixels];

// WiFi and WebSocket configuration
const char* ssid = "Etienne";
const char* password = "etienne123";
String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
String wsServer = "ws://192.168.176.96:3000"; // WebSocket server address

WebsocketsClient wsClient;

void setup() {
  Serial.begin(9600);
  setupPins();
  setupWifi();
  connectWebSocket();
}

void loop() {
  // Handle WebSocket events
  wsClient.poll();
}

void setupPins() {
  pinMode(LEDPin, OUTPUT);
  pinMode(valvePinA, OUTPUT);
  pinMode(valvePinB, OUTPUT);
}

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

void connectWebSocket() {
  // Connect to WebSocket server
  Serial.println("Connecting to WebSocket server...");
  wsClient.onMessage(onWebSocketMessage); // Set callback for received messages
  wsClient.onEvent(onWebSocketEvent);     // Set callback for WebSocket events
  wsClient.connect(wsServer);

  if (wsClient.available()) {
    Serial.println("WebSocket connected!");
  } else {
    Serial.println("WebSocket connection failed.");
  }
}

void onWebSocketMessage(WebsocketsMessage message) {
  // Handle received messages
  Serial.println("Message received: ");
  Serial.println(message.data());

  // Parse the received message (assuming JSON format)
  JSONVar json = JSON.parse(message.data());
  if (JSON.typeof(json) == "undefined") {
    Serial.println("Invalid JSON received");
    return;
  }

  // Check if the message is for this device
  if (String(json["deviceId"]) == deviceID) {

    // Check if the message is concerning a potential seizure pose that has been detected
    int pose = (const int)json["pose"];
    Serial.print("Pose: ");
    Serial.println(pose);

    if (pose >= 0 && pose < 7) {
      handleMotor(true); // Turn motor ON
    }

    // String status = (const char*)json["status"];
    // Serial.print("Status: ");
    // Serial.println(status);

    // if (status == "ON") {
      
    // } else if (status == "OFF") {
    //   handleMotor(false); // Turn motor OFF
    // } else if (status == "LowBattery") {
    //   Serial.println("Low battery warning!");
    //   handleLowBattery();
    // }
  } else {
    Serial.println("Message not intended for this device.");
  }
}

void onWebSocketEvent(WebsocketsEvent event, String data) {
  switch (event) {
    case WebsocketsEvent::ConnectionOpened:
      Serial.println("WebSocket connection opened.");
      break;
    case WebsocketsEvent::ConnectionClosed:
      Serial.println("WebSocket connection closed.");
      break;
    case WebsocketsEvent::GotPing:
      Serial.println("WebSocket Ping!");
      break;
    case WebsocketsEvent::GotPong:
      Serial.println("WebSocket Pong!");
      break;
  }
}

void handleMotor(bool onOff) {
  if (onOff) {
    digitalWrite(valvePinA, HIGH);
    digitalWrite(valvePinB, HIGH);
    Serial.println("Motor turned ON");
  } else {
    digitalWrite(valvePinA, LOW);
    digitalWrite(valvePinB, LOW);
    Serial.println("Motor turned OFF");
  }
}

void handleLowBattery() {
  // Flash LED to indicate low battery
  for (int i = 0; i < 10; i++) {
    digitalWrite(LEDPin, HIGH);
    delay(250);
    digitalWrite(LEDPin, LOW);
    delay(250);
  }
  Serial.println("Low battery LED warning completed.");
}
