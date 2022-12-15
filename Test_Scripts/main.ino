#include <Arduino.h>
#include "sensor.h"


void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
}

void loop() {
    readSensor( 102, 800);
    delay(5000);
}