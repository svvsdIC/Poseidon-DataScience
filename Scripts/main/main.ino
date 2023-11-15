


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
    > (done) Investigate sensor control-board internal memory
        > (done) Read and write files reliably on μSD card
    > .csv file generation
        > (done) Generate CSV file content 
        > (done) Create and modify CSV files ion microSD card
        > Add logging of recorded data to main.ino 
    > Add More Methods for sensors
        > Sleep
            >  Place at
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
#include <SPI.h>
#include <SD.h>
#include <cmath>
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

const int cardSelect = 4;

// the name of the file on the μsd card where the data are recorded
char logFileName[MAX_FILE_NAME_LENGTH + 1];


void setup() {

    // Start serial and I2C connections, initialize sensors

    Serial.begin(9600);
    Serial.println("Serial Initialized");

    Wire.begin();

    while (!SD.begin(cardSelect)) {
        Serial.println("ERROR: μSD card initialization failed!");
        delay(500);
    }

    delay(500);

    //final header to protect from overflows and fit nicely in our functions
    char final_csv_header_string[MAX_CSV_ROW_LENGTH + 1] = "Timestamp,Sensor #,Value";

    for(Sensor_Base obj : allSensorInstances) {
        obj.enableAllParameters();

    }

    createLogFile(final_csv_header_string, logFileName); // TODO: add error handling

    
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
            Serial.print("ERROR: Sensor error.  Status Code: ");
            Serial.print(responseCode);
            Serial.print(" on sensor: ");
            Serial.println(obj.m_displayNames[0]);
            continue;
        }

        // The data from one cycle through the sensors
        char single_data_line[MAX_CSV_ROW_LENGTH + 1];
        

        for(int i = 0; (returnedValues[i].type != INVALID_TYPE) && (returnedValues[i].timeStamp != 0); i++) {

            // sends results back over serial line
            Serial.print("At time: ");
            char timeStampString[MAX_TIME_CHARS + 1];
            formatTime(returnedValues->timeStamp, timeStampString);
            Serial.print(timeStampString);
            Serial.print(", ");
            Serial.print(obj.m_displayNames[i]);
            Serial.print(" measured: ");
            Serial.println(returnedValues[i].value);


            // csv rows are "Timestamp,Sensor #,Value"

            char single_csv_line[MAX_CSV_ROW_LENGTH + 1];
            char csv_values[MAX_CSV_ROW_LENGTH + 1];

            strncpy(single_csv_line, timeStampString, MAX_CSV_ROW_LENGTH);

            // add the timestamp to the type and value of each reading in csv format
            sprintf(csv_values, ",%0i,%0f", (int) returnedValues[i].type, returnedValues[i].value);
            strncat(single_csv_line, csv_values, MAX_CSV_ROW_LENGTH);

            // record data on SD card
            logData(single_csv_line, logFileName);

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