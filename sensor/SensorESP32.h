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
    int ranges[5];  // Stores the ranges requested from the server
    UltrasonicSensor sensor;  // Instance of the Sensor class for measurement
    WiFiClient client;  // WiFi client

public:
    SensorESP32(const char* ssid, const char* password, const char* host, int port, unsigned int triggerPin, unsigned int echoPin)
        : ssid(ssid), password(password), host(host), port(port), lastState(-1), sensor(triggerPin, echoPin) {}

    // Connect to WiFi network
    void connectToWiFi() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to WiFi");
    }

    // Attempt to connect to the server
    bool connectToServer() {
        if (client.connect(host, port)) {
            Serial.println("Connected to server.");
            return true;
        } else {
            Serial.println("Failed to connect to server.");
            return false;
        }
    }

    // Request ranges from the server
    void requestRanges() {
        if (client.connected()) {
            client.println("GET_RANGES");  // Request ranges from server
            String data = client.readStringUntil('\n');
            if (sscanf(data.c_str(), "%d,%d,%d,%d,%d", &ranges[0], &ranges[1], &ranges[2], &ranges[3], &ranges[4]) == 5) {
                Serial.println("Ranges received: ");
                for (int i = 0; i < 5; i++) {
                    Serial.println(ranges[i]);
                }
            } else {
                Serial.println("Error receiving ranges.");
            }
        } else {
            Serial.println("Not connected to server.");
        }
    }

    // Calculate state based on measured distance and ranges
    int calculateState(int distance) {
        if (distance > ranges[0]) {
            return 0;  // Greater than the highest range
        }
        for (int i = 1; i < 5; i++) {
            if (distance <= ranges[i]) {
                return i;  
            }
        }
        return 0;  // Fallback in case of unexpected error
    }

    // Read distance from sensor and send data to server
    void sendData() {
        int distance = sensor.readDistanceInCM();  // Read the distance
        int newState = calculateState(distance);

        // Only send the state if it has changed
        if (newState != lastState) {
            if (client.connected()) {
                client.println(newState);  // Send new state to server
                Serial.println("State sent: " + String(newState));
                lastState = newState;
            } else {
                Serial.println("Not connected to server to send state.");
            }
        }
    }

    // Handle reconnection to server in case of disconnection
    void handleConnections() {
        if (!client.connected()) {
            Serial.println("Reconnecting to server...");
            connectToServer();
            requestRanges();  // Request ranges again upon reconnection
        }
    }

    // Check if WiFi connection is lost and reconnect if necessary
    void checkWiFi() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Reconnecting to WiFi...");
            connectToWiFi();
        }
    }
};

#endif
