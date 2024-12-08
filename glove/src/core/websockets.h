#include <ArduinoWebsockets.h>
#include <Arduino_JSON.h>

using namespace websockets;

class Websocket {
  WebsocketsClient wsClient;
  String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
  String wsServer = "ws://192.168.176.124:3000";


  public:
    void onWebSocketMessage(WebsocketsMessage message) {
      Serial.println("Message received: ");
      Serial.println(message.data());
    }

    void sendMessage(String message) {
      wsClient.send(message);
    }

    void connect() {
      Serial.println("Connecting to WebSocket server...");
      // wsClient.onMessage(onWebSocketMessage);
      wsClient.connect(wsServer);

      if (wsClient.available()) {
        Serial.println("WebSocket connected!");
      } else {
        Serial.println("WebSocket connection failed.");
      }
    }

    void poll() {
      if (!wsClient.available()) {
        Serial.println("WebSocket disconnected. Reconnecting...");
        connect();
      }
      wsClient.poll();
    }
};