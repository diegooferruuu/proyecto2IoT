#include <WiFi.h>
#include <WiFiClient.h>
#include "LedController.h"

class ActuatorESP32 {
private:
    const char* ssid;
    const char* password;
    const char* serverIP;
    int serverPort;
    int lastNumber;
    WiFiClient client;
    LedController ledController;

public:
    ActuatorESP32(const char* ssid, const char* password, const char* serverIP, int serverPort, int pin1, int pin2, int pin3, int pin4)
        : ssid(ssid), password(password), serverIP(serverIP), serverPort(serverPort), lastNumber(-1), ledController(pin1, pin2, pin3, pin4) {}

    void setup() {
        Serial.begin(115200);
        ledController.initialize();

        connectToWiFi();
        connectToServer();
    }

    void loop() {
        if (client.connected()) {
            if (client.available()) {
                String receivedData = client.readStringUntil('\n');
                int receivedNumber = receivedData.toInt();

                if (receivedNumber != lastNumber) {
                    Serial.print("Number received: ");
                    Serial.println(receivedNumber);
                    ledController.turnOnLeds(receivedNumber);
                    lastNumber = receivedNumber;
                }
            }
        } else {
            reconnect();
        }
    }

private:
    void connectToWiFi() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to WiFi network");
    }

    void connectToServer() {
        if (!client.connect(serverIP, serverPort)) {
            Serial.println("Failed to connect to server");
        } else {
            Serial.println("Connected to server");
        }
    }

    void reconnect() {
        Serial.println("Connection lost, attempting to reconnect...");
        if (!client.connect(serverIP, serverPort)) {
            Serial.println("Error reconnecting to server");
            delay(1000);
        }
    }
};

