#include <WiFi.h>
#include "UltrasonicSensor.h"
#include "SensorESP32.h"

SensorESP32 sensorESP32("LABO17", "catolica17", "192.168.77.210", 12345, 22, 34);

void setup() {
    Serial.begin(9600);  
    sensorESP32.connectToWiFi(); 

    if (sensorESP32.connectToServer()) {
        sensorESP32.requestRanges(); 
    }
}

void loop() {
    sensorESP32.sendData(); 
    delay(100); 
}
