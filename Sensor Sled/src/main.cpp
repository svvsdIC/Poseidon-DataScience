


/*

File Description:

    Instantiates sensors, performs looped reads of each one.  
    Currently serves as a testbed for sensor.cpp and sensor.h

*/

/*

Perpetual goals:
> Find out/develop concrete use-cases for sensor package
    > Use cases for end-user


Development Roadmap:

> (done) Better error handling in and out of methods 
> (done) Hours/Minutes/Seconds timestamp display
> (done) Investigate sensor control-board internal memory
    > (done) Read and write files reliably on μSD card
> (done) .csv file generation
    > (done) Generate CSV file content 
    > (done) Create and modify CSV files ion microSD card
    > (done) Add logging of recorded data to main.ino 


> Create Serial Interface
    > (done) Create infrastructure
    > Design all serial commands
        > Set Time
        > Write to csv
        > User-selected measurment delivery modes
            > Single read
            > Periodic reads
            > etc.
        > etc.

    > Implement serial commands
        > Add More Methods for sensors
            > Sleep
            > Return current status
            > etc.

> Write documentation for existing structures

> Write serial command docs

> Misc
    > Internal Battery management
        > Measure voltage

> Light Sensors
    > Figure out surface-relative reading configuration
    > Design and implement sensors' structures and methods

> Turbidity sensor 
    > Design and build turbidity sensor
        > Allocate reading GPIO pins on Feather
    > Design and implement sensor's structures and methods



Links:

    Sensor Use case document:
    https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

    Sensor Test and Calibration Document:
    https://docs.google.com/document/d/1IG6QW2--BlNknFqRsbM8qlOfcKgexicl__1FPq2o_nE/edit#heading=h.ny5vp0ghyued

    Data Science team document:

    https://docs.google.com/document/d/12zM-jy_i1q-XmQzXvvwT4mxrKElRtTjQt7xu8XVNhUw

    Documentation and Resources:

    https://drive.google.com/drive/folders/1I4Cdi6U9HW2RpAAhkCUK7W7IaoSDOxND?usp=drive_link

*/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "sensor.h"
#include "utilities.h"
#include "eventlogger.h"
#include "serialcommand.h"

// PLACEHOLDER FUNCTION
void read_all_sensors();

// The string recieved over the serial line
char serialCommand[MAX_SERIAL_COMMAND_LENGTH + 1];

// True when there is a complete command recieved over the serial line, to be executed
bool serialCommandReady = false;

// Defines the static variables used to keep track of sensor objects
int Sensor_Base::m_numberOfSensors = 0;

// Array of pointers to each sensor
// Array is populated when a Sensor_Base constructor runs
Sensor_Base* Sensor_Base::m_ListOfSensorObjects[MAX_NUMBER_OF_SENSORS] = {};

// Event logger object
Event_Logger obj_EventLogger = Event_Logger("EVENTLOG.txt", false);



// Create an instance of each sensor
Sensor_EC obj_EC = Sensor_EC();
Sensor_DO obj_DO = Sensor_DO();
Sensor_TEMP obj_TEMP = Sensor_TEMP();
Sensor_PH obj_PH = Sensor_PH();
Sensor_OR obj_OR = Sensor_OR();

// Which pin serves as the SD card select
const int cardSelect = 4;

// Wehther the sensor package program has an active serial connection
bool initialized_with_serial = false;

// The name of the file on the μsd card where the data are recorded
char dataFileName[MAX_FILE_NAME_LENGTH];



void setup() {

    // Initialize μSD card
    while (!SD.begin(cardSelect)) {
        if(obj_EventLogger.m_sendLogsOnSerial) {
            // TODO: LED flash
        }
        delay(500);
    }

    createDataFile(SENSOR_CSV_HEADER, dataFileName); // TODO: add error handling


    //Log startup event
    obj_EventLogger.LogEvent("\n******************************\nProgram Start");

    // initialize serial connection, or skip after 20 seconds
    while(millis() < 20000) {
        if(Serial) {
            initialized_with_serial = true;
            obj_EventLogger.m_sendLogsOnSerial = true;
            break;
        }
    }

    if(initialized_with_serial) {
        Serial.begin(9600);
        obj_EventLogger.LogEvent("Initialized with serial connection");

    } else {
        obj_EventLogger.LogEvent("Initialized without serial connection");
    }


    // Start I2C
    Wire.begin();

    // Activate reading for all readingtypes of each sensor
    for(Sensor_Base * obj : Sensor_Base::m_ListOfSensorObjects) {

        if(!obj) {
            break;
        }

        obj->enableAllParameters();
    }
    
}

void loop() {

    // For recieving comands over serial line
    if(Serial.available()) {

        int currentByte = 0;

        while( (Serial.available())) {
            serialCommand[currentByte] = Serial.read();

            currentByte++;

            if (currentByte >= MAX_SERIAL_COMMAND_LENGTH + 1)  {
                obj_EventLogger.LogError("Serial command exceeded maximum number of characters");
                currentByte = 0;
                Serial.flush();
                break; 
            }

        }

        if(serialCommand[currentByte - 1] == '\r') {

            serialCommand[currentByte - 1] = '\0';

            currentByte = 0;
        
            // TODO: add command parser here

            // test echo
            Serial.print("Command recieved: ");
            Serial.println(serialCommand);
            Serial.println("Did: Nothing");
            
        }

        

    }
    

   if(!initialized_with_serial) {
        //default behavior without serial command
        read_all_sensors();
        delay(4000);
   } else {
        // TODO: replace with response to active serial command
        read_all_sensors();

        delay(4000);
   }

}


void read_all_sensors() {
    for(Sensor_Base *obj : Sensor_Base::m_ListOfSensorObjects) {

        if(!obj) {
            break;
        }


        SensorValue returnedValues[MAX_READINGS_PER_SENSOR + 1];

        for(int i = 0; i < (int) (sizeof(returnedValues) / sizeof(returnedValues[0])); i++) {
            returnedValues[i].timeStamp = 0;
            returnedValues[i].value = 0;
            returnedValues[i].type = INVALID_TYPE;
        }
        
        int responseCode = obj->read(returnedValues);

        if(responseCode != 1) {

            char errorLine[MAX_FILE_ROW_LENGTH + 1];
            
            snprintf(errorLine,
                        MAX_FILE_ROW_LENGTH,
                        "Sensor Response Code: %i on sensor: %s",
                        responseCode,
                        obj->m_displayNames[0]);

            obj_EventLogger.LogError(errorLine);

            continue;
        }        

        for(int i = 0; (returnedValues[i].type != INVALID_TYPE); i++) {
              
            char timeStampString[MAX_TIME_CHARS + 1];
            formatTime(returnedValues->timeStamp, timeStampString);
        
            if(obj_EventLogger.m_sendLogsOnSerial) {
                Serial.print("At time: ");
                Serial.print(timeStampString);
                Serial.print(", ");
                Serial.print(obj->m_displayNames[i]);
                Serial.print(" measured: ");
                Serial.println(returnedValues[i].value);
            }


            // csv rows are "Timestamp,Reading Type,Value"

            char single_csv_line[MAX_FILE_ROW_LENGTH + 1];
            char csv_values[MAX_FILE_ROW_LENGTH + 1];

            strncpy(single_csv_line, timeStampString, MAX_FILE_ROW_LENGTH);

            char valueString[MAX_FILE_ROW_LENGTH + 1];

            

            // add the timestamp to the type and value of each reading in csv format
            sprintf(csv_values, ",%s,%s", (obj->m_displayNames[i]), String(returnedValues[i].value, 5).c_str());


            strncat(single_csv_line, csv_values, MAX_FILE_ROW_LENGTH);


            // record data on SD card
            writeLineToFile(single_csv_line, dataFileName);

        }        


        Serial.print("\n");

        delay(1000);
    }
}
