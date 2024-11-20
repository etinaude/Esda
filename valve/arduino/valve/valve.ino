#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <Arduino_JSON.h>
#include "FastLED.h"

using namespace websockets;

#define valvePinA 9
#define valvePinB 10

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
  if (!wsClient.available()) {
    Serial.println("WebSocket disconnected. Reconnecting...");
    connectWebSocket();
  }
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
    int pose = (int)json["pose"];
    Serial.print("Pose: ");
    Serial.println(pose);

    if (pose >= 0 && pose < 7)
    {
      handleValve(true); // Turn valve ON
    } 

    String event = (const char*)json["event"];
    Serial.print("event: ");
    Serial.println(event);

    if (event == "cancel")
    {
      handleValve(false); // Turn valve OFF
    } 
    

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

void handleValve(bool on) {
  if (on) 
  {
    Serial.println("Turning valve ON...");

    digitalWrite(valvePinA, HIGH);
    delay(500); 
    digitalWrite(valvePinA, LOW);

    Serial.println("Valve is ON");
  } 
  else 
  {
    Serial.println("Turning valve OFF...");

    digitalWrite(valvePinB, HIGH);
    delay(500);
    digitalWrite(valvePinB, LOW);

    Serial.println("Valve is OFF");
  }
}

