#ifndef SENSOR_ESP32_H
#define SENSOR_ESP32_H

#include <WiFi.h>
#include "UltrasonicSensor.h"

class SensorESP32 {
private:
    const char* ssid;
    const char* password;
    const char* host;
    int port;
    int lastState;
    int ranges[4];  
    UltrasonicSensor sensor;  
    WiFiClient client; 

public:
    SensorESP32(const char* ssid, const char* password, const char* host, int port, unsigned int triggerPin, unsigned int echoPin)
        : ssid(ssid), password(password), host(host), port(port), lastState(-1), sensor(triggerPin, echoPin) {}

    void connectToWiFi() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to WiFi");
    }

    bool connectToServer() {
        if (client.connect(host, port)) {
            Serial.println("Connected to server.");
            return true;
        } else {
            Serial.println("Failed to connect to server.");
            return false;
        }
    }

    void requestRanges() {
        if (client.connected()) {
            client.println("GET_RANGES");
            String data = client.readStringUntil('\n');
            if (sscanf(data.c_str(), "%d,%d,%d,%d", &ranges[0], &ranges[1], &ranges[2], &ranges[3]) == 4) {
                Serial.println("Ranges received: ");
                for (int i = 0; i < 4; i++) {
                    Serial.println(ranges[i]);
                }
            } else {
                Serial.println("Error receiving ranges.");
            }
        } else {
            Serial.println("Not connected to server.");
        }
    }

    int calculateState(int distance) {
        if (distance > ranges[3]) {
            return 0; 
        }
        for (int i = 0; i < 4; i++) {
            if (distance <= ranges[i]) {
                return i+1;  
            }
        }
        return 0;
    }

    void sendData() {
        int distance = sensor.readDistanceInCM();
        int newState = calculateState(distance);

        if (newState != lastState) {
            if (client.connected()) {
                client.println(newState); 
                Serial.println("State sent: " + String(newState));
                lastState = newState;
            } else {
                Serial.println("Not connected to server to send state.");
            }
        }
    }

    void handleConnections() {
        if (!client.connected()) {
            Serial.println("Reconnecting to server...");
            connectToServer();
            requestRanges();
        }
    }

    void checkWiFi() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Reconnecting to WiFi...");
            connectToWiFi();
        }
    }
};

#endif
