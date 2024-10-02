#include <WiFi.h>
#include <WiFiClient.h>

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
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {  // Limita el número de intentos
        delay(2000);  // Tiempo de espera más largo
        Serial.print("Attempting to connect to WiFi, attempt: ");
        Serial.println(attempts + 1);
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi network");
    } else {
        Serial.println("Failed to connect to WiFi");
    }
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

ActuatorESP32 actuator("H_V", "alberto123", "192.168.155.247", 12345, 16, 4, 5, 18);

void setup() {
    actuator.setup();
}

void loop() {
    actuator.loop();
}