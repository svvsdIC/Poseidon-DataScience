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

    {98, 815, "OR", ""},/* [status][ascii encoded signed float mV][null] */
    {99, 815, "pH", ""}, /* [status][ascii encoded floating point pH][null] */
    {97, 575, "DO", ""}, /* [status][ascii encoded float mg / L and %][null] */
    {100, 800, "EC", ""}, /* [status][ascii encoded floats μS / cm, val2, val3, val4][null] */
    {102, 600, "RT", ""}  /* [status][ascii encoded float (3 decimals) celsius][null] */
};


#define SENSOR_STORAGE_BUFFER_SIZE (256)

struct SensorOutputArray {


    int nothing;
    SensorOutput sensorOutputs[];

};

SensorOutputArray sensorOutputs[] = {

    SensorOutput OR[SENSOR_STORAGE_BUFFER_SIZE],
    SensorOutput pH[SENSOR_STORAGE_BUFFER_SIZE],
    SensorOutput DO[SENSOR_STORAGE_BUFFER_SIZE],
    SensorOutput EC[SENSOR_STORAGE_BUFFER_SIZE],
    SensorOutput RT[SENSOR_STORAGE_BUFFER_SIZE]

};

//sensorOutputData[]

#define RECIEVED_BUFFER_SIZE (32)

char serialCommand[RECIEVED_BUFFER_SIZE];
int currentByte = 0;
bool serialCommandReceived = false;


void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized :)");
    initSensors();
}

void loop() {

    if(serialCommandReceived) {
        // follow command
        serialCommandReceived = false;
    }


    /*readSensor(sensors[ph]);
    Serial.println(sensors[ph].outputData); */

    //delay(1000);
    for (int i = 0; i < 5; i++) {

        readSensor(sensors[i]);
        Serial.print(sensors[i].sensorName);
        Serial.print(": ");
        Serial.println(sensors[i].outputData);

        delay(2500);
    } 


}

void serialEvent() { // called after loop() when there is data in the buffer
    while(Serial.available()) {
        serialCommand[currentByte] = Serial.read();
        currentByte++;
    }
    serialCommandReceived = true;
}