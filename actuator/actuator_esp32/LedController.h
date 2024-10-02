#include <Arduino.h>

class LedController {
private:
    int ledPins[4];

public:
    LedController(int pin1, int pin2, int pin3, int pin4) {
        ledPins[0] = pin1;
        ledPins[1] = pin2;
        ledPins[2] = pin3;
        ledPins[3] = pin4;
    }

    void initialize() {
        for (int i = 0; i < 4; i++) {
            pinMode(ledPins[i], OUTPUT);
            digitalWrite(ledPins[i], LOW);
        }
    }

    void turnOnLeds(int num) {
        switch (num) {
            case 1:
                digitalWrite(ledPins[0], HIGH);
                digitalWrite(ledPins[1], HIGH);
                digitalWrite(ledPins[2], HIGH);
                digitalWrite(ledPins[3], HIGH);
                break;
            case 2:
                digitalWrite(ledPins[0], HIGH);
                digitalWrite(ledPins[1], HIGH);
                digitalWrite(ledPins[2], HIGH);
                digitalWrite(ledPins[3], LOW);
                break;
            case 3:
                digitalWrite(ledPins[0], HIGH);
                digitalWrite(ledPins[1], HIGH);
                digitalWrite(ledPins[2], LOW);
                digitalWrite(ledPins[3], LOW);
                break;
            case 4:
                digitalWrite(ledPins[0], HIGH);
                digitalWrite(ledPins[1], LOW);
                digitalWrite(ledPins[2], LOW);
                digitalWrite(ledPins[3], LOW);
                break;
            case 0:
            default:
                digitalWrite(ledPins[0], LOW);
                digitalWrite(ledPins[1], LOW);
                digitalWrite(ledPins[2], LOW);
                digitalWrite(ledPins[3], LOW);
                break;
        }
    }
};

