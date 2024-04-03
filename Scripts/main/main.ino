


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
#include "sensor.h"
#include "utilities.h"
#include "eventlogger.h"
#include "serialcommand.h"

// For recieving commands over serial line

char serialCommand[MAX_SERIAL_COMMAND_LENGTH + 1];
bool serialCommandReady = false;
int currentByte = 0;

// Defines the static variables used to keep track of sensor objects
int Sensor_Base::m_numberOfSensors = 0;

Sensor_Base* Sensor_Base::m_ListOfSensorObjects[MAX_NUMBER_OF_SENSORS] = {};

Event_Logger obj_EventLogger = Event_Logger("EVENTLOG.txt", false);

// Create an instance of each sensor

Sensor_EC obj_EC = Sensor_EC();
Sensor_DO obj_DO = Sensor_DO();
Sensor_TEMP obj_TEMP = Sensor_TEMP();
Sensor_PH obj_PH = Sensor_PH();
Sensor_OR obj_OR = Sensor_OR();

//Sensor_Base allSensorInstances[] = {obj_OR, obj_EC, obj_DO, obj_PH, obj_TEMP };



const int cardSelect = 4;

bool initialized_with_serial = false;

// the name of the file on the μsd card where the data are recorded
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

    // initialize serial connection or skip after 20 seconds
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

    Serial.println("made it to end of setup");
    
}

void loop() {

    Serial.println("made it to start of looop");

    // For recieving comands over serial line
    if(Serial.available()) {

        currentByte = 0;

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
