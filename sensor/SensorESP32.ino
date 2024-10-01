#include <WiFi.h>
#include "UltrasonicSensor.h"
#include "SensorESP32.h"

SensorESP32 sensorESP32("LABO17", "catolica17", "192.168.77.210", 12345, 22, 34);  // Sensor pins (trigger and echo)

void setup() {
    Serial.begin(115200);  // Initialize serial monitor
    sensorESP32.connectToWiFi();  // Connect to WiFi network

    // Attempt to connect to the server at startup
    if (sensorESP32.connectToServer()) {
        sensorESP32.requestRanges();  // Request ranges from the server
    }
}

void loop() {
    sensorESP32.sendData();  // Send data only if the state changes
    delay(100);  // Wait before the next measurement
}
