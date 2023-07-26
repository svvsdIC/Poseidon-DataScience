/*

File Description:

    Instantiates sensors, performs looped reads of each one.  
    Currently serves as a testbed for sensor.cpp and sensor.h

*/

/*

Development Roadmap:

    Immediate Goals:

    > (done) Better error handling in and out of methods 
    > (done) Hours/Minutes/Seconds timestamp display
    > Investigate sensor control-board internal memory
    > .csv file generation    
    > Add More Methods for sensors
        > Full Sleep mode
        > Return current status
        > etc.

    
    > Build prototype ring buffer


    Mid-term Goals:

    > User-selected measurment delivery modes
        > 1 read
        > Periodic reads
        > Depth-activated reads
        > etc.
    > Internal Battery management
        > Measure voltage
    > Build prototype UI (terminal-style commands over serial)


    Long-Term Goals:

    > Find out/develop concrete use-cases for sensor package
        > Use cases for to end-user
    > Decide on delivery to user
        > SD card/Internal Storage
        > Serial
        > Data Radio
        > Bluetooth connection
        > etc.


Links:

    Sensor Use case document:
    https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

    Sensor Test and Calibration Document:
    https://docs.google.com/document/d/1IG6QW2--BlNknFqRsbM8qlOfcKgexicl__1FPq2o_nE/edit#heading=h.ny5vp0ghyued

    Data Science team document:

    https://docs.google.com/document/d/12zM-jy_i1q-XmQzXvvwT4mxrKElRtTjQt7xu8XVNhUw


*/

#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"
#include "utilities.h"

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
            Serial.print(responseCode);
            Serial.print(" on sensor: ");
            Serial.println(obj.m_displayNames[0]);
            continue;
        }

        for(int i = 0; (returnedValues[i].type != INVALID_TYPE) && (returnedValues[i].timeStamp != 0); i++) {

            Serial.print("At time: ");
            char timeStampString[MAX_TIME_CHARS + 1];
            formatTime(returnedValues->timeStamp, timeStampString);
            Serial.print(timeStampString);
            Serial.print(", ");
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