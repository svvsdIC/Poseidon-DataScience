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

/* Address - I2C bus adress
eadDelay - duration 
sensorName - 3 character name of the sensor
*/

enum sensorNames {
    oxygenReduction,
    ph,
    dissolvedOxygen,
    electricalConductivity,
    temperature
};

    struct AtlasSensor
{
    int address;
    int readDelayMS;
    String sensorName;
};
AtlasSensor sensor[] = {
    {98, 815, "OR"},
    {99, 815, "pH"},
    {97, 575, "DO"},
    {100, 570, "EC"},
    {102, 600, "RT"}
    }; 

void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
}

void loop() {
    readSensor(102, 800);
    delay(5000);
}