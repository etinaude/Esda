#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

String ssid = "Etienne";
String password = "etienne123";
String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
String serverName = "http://192.168.176.124:3000";

class Api {
  public:
    Api() {}

    void setupWifi() {
      WiFi.begin(ssid, password);
      Serial.println("Connecting");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());

      delay(1000);
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

    void sendDetectApi(int mode, float confidence) {
      JSONVar data;

      data["accuracy"] = confidence;
      data["pose"] = mode;

      // send api
      sendAPI("/detect", data);
    }

    void sendTrainApi(JSONVar data) {
      // send api
      sendAPI("/train", data);
    }



    void sendStatusApi() {
      // TODO
    }

    void sendCancelApi() {
      JSONVar data;
      sendAPI("/cancel", data);
    }
};