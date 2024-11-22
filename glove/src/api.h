#include <WiFi.h>
#include <HTTPClient.h>

String ssid = "Etienne";
String password = "etienne123";
String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
String serverName = "http://192.168.176.96:3000";

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void sendAPI(String endPoint, JSONVar data) {
  data["deviceId"] = deviceID;
  String jsonString = JSON.stringify(data);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName + endPoint);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonString);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
  }
}