#include "ActuatorESP32.h"

ActuatorESP32 actuator("LABO17", "catolica17", "192.168.77.210", 12345, 16, 4, 5, 18);

void setup() {
    actuator.setup();
}

void loop() {
    actuator.loop();
}