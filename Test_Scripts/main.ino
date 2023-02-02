#include <Arduino.h>
#include "sensor.h"

// Sensor Use cases documented here:
// https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

/* TODO:
[] low power mode - deep sleep
[] detect sensor error condition
[] onboard logging
[] stream live data
[] instruct single read
[] Current/voltage measurement
[] Bluetooth connection
[] Take readings on command
[] Measure with set delay
[] Detect water depth
[] Data Radio (one measure/hour)
[] Detect Error conditions
[] Local data storage
*/

/* address - I2C bus adress
readDelayMS - duration of read delay in milliseconds
sensorName - 3 character name of the sensor
*/

AtlasSensor sensors[] = {
    {98, 815, "OR"},
    {99, 815, "pH"},
    {97, 575, "DO"},
    {100, 570, "EC"},
    {102, 600, "RT"}
};

char serialCommands[20];
int currentByte = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
}

void loop() {


    /*for (int i = 0; i < 5; i++) {
        Serial.print(sensors[i].sensorName);
        Serial.print(": ");
        Serial.println(readSensor(sensors[i]));

        delay(2500);
    }*/


}

void serialEvent() {
    while(Serial.available()) {
        serialCommands[currentByte] = Serial.read();
        currentByte++;
    }
}