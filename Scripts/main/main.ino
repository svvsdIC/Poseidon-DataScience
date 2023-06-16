// Sensor Use cases documented here:
// https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

/* TODO:
[] constantly read unless told to turn off, return values when asked for
[] Hours/Minutes/Seconds since start
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

#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"


// For recieving commands over serial line
/*
#define RECIEVED_BUFFER_SIZE (32)
char serialCommand[RECIEVED_BUFFER_SIZE];
int currentByte = 0;
bool serialCommandReceived = false;
*/


// Create an instance of each sensor
Sensor_EC obj_EC = Sensor_EC();
Sensor_DO obj_DO = Sensor_DO();
Sensor_TEMP obj_TEMP = Sensor_TEMP();
Sensor_PH obj_PH = Sensor_PH();
Sensor_OR obj_OR = Sensor_OR();

Sensor_Base allSensorInstances[] = {obj_OR, obj_EC, obj_DO, obj_PH, obj_TEMP };

void setup() {

    // Start serial and I2C connections, initialize sensors

    Serial.begin(9600);
    Serial.println("Serial Initialized");

    Wire.begin();

    delay(500);

    for(Sensor_Base obj : allSensorInstances) {
        obj.enableAllParameters();
    }
    
}

void loop() {

    // For recieving commands over serial line
    /*
    if(serialCommandReceived) {
        // follow command
        serialCommandReceived = false;
    }
    */


    // read each sensor in sequence and print out the returned value(s)
    for(Sensor_Base obj : allSensorInstances) {

        SensorValue returnedValues[MAX_READINGS_PER_SENSOR + 1];

        for(int i = 0; i < (int) (sizeof(returnedValues) / sizeof(returnedValues[0])); i++) {
            returnedValues[i].timeStamp = 0;
            returnedValues[i].value = 0;
            returnedValues[i].type = INVALID_TYPE;
            
        }
        
        int responseCode = obj.read(returnedValues);
        if(responseCode != 1) {
            Serial.print("ERROR: Status Code: ");
            Serial.println(responseCode);
            continue;
        }

        for(int i = 0; (returnedValues[i].type != INVALID_TYPE) && (returnedValues[i].timeStamp != 0); i++) {

            Serial.print(obj.m_displayNames[i]);
            Serial.print(" measured: ");
            Serial.println(returnedValues[i].value);
           
        }

        Serial.print("\n");

        delay(1000);
    }

    delay(4000);

}



// For recieving commands over serial line
/*
void serialEvent() { // called after loop() when there is data in the buffer
    while(Serial.available()) {
        serialCommand[currentByte] = Serial.read();
        currentByte++;
    }
    serialCommandReceived = true;
}
*/