#include <Arduino.h>
#include "sensor.h"

// Sensor Use cases documented here:
// https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

/* TODO:
[] low power mode - deep sleep
[] detect sensor error condition
[] onboard logging
    [] look at built-in sensor storage
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

AtlasSensor sensors[] = {

    /*
    {address, readDelayMS, sensorName}

    address - I2C bus adress
    readDelayMS - duration of read delay in milliseconds
    sensorName - 3 character name of the sensor
    */

    {98, 815, "OR"},/* [status][ascii encoded signed float mV] */
    {99, 815, "pH"}, /* [status][ascii encoded floating point pH][null] */
    {97, 575, "DO"}, /* [status][ascii encoded float mg / L][null] */
    {100, 1000, "EC"}, /* [status][ascii encoded float μS / cm; comma between 1000s place?][null] */
    {102, 600, "RT"}  /* [status][ascii encoded float (3 decimals) celsius][null] */
};

char serialCommand[20];
int currentByte = 0;
bool serialCommandReceived = false;


void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
}

void loop() {

    if(serialCommandReceived) {
        // follow command
        serialCommandReceived = false;
    }


    readSensor(sensors[electricalConductivity]);


    /*for (int i = 0; i < 5; i++) {
        Serial.print(sensors[i].sensorName);
        Serial.print(": ");
        Serial.println(readSensor(sensors[i]));

        delay(2500);
    }*/


}

void serialEvent() { // called after loop() when there is data in the buffer
    while(Serial.available()) {
        serialCommand[currentByte] = Serial.read();
        currentByte++;
    }
    serialCommandReceived = true;
}