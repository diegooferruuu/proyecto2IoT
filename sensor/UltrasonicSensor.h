#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

class UltrasonicSensor {
private:
    unsigned int triggerPin;
    unsigned int echoPin;

public:
    UltrasonicSensor(unsigned int triggerPin, unsigned int echoPin) 
        : triggerPin(triggerPin), echoPin(echoPin) {
        pinMode(triggerPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }

    float readDistanceInCM() {
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        return pulseIn(echoPin, HIGH) * 0.01723;  // Conversion to cm
    }
};

#endif
