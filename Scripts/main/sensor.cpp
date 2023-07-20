/*

File Description:

    Defines the methods and constructors for running AtlasSensors using the Sensor_Base
    class and its derivatives.

*/

#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"



// ===== Methods for Sensor_Base Class ====================================================================================================================================================


// Constructor for the Sensor_Base class
Sensor_Base::Sensor_Base(int address, unsigned long readDelayMS) {

    m_address = address;
    m_readDelayMS = readDelayMS;

    for(ReadingType x : m_readingTypes) {
        x = (ReadingType) INVALID_TYPE;
    }
    
    for(int i = 0; i < MAX_READINGS_PER_SENSOR; i++) {
        this->m_displayNames[i][0] = 0x00;
    }
        
}


// Reads the specified sensor, returns the status code,
// and fills in referenced SensorValue array
int Sensor_Base::read(SensorValue (&outputLocation)[MAX_READINGS_PER_SENSOR + 1]) {

    char cmd[] = "r"; 
    int responseCode = this->sendI2CMessage(cmd);

    if(responseCode != SUCCESS) {
        return responseCode;
    }

    double separatedSensorValues[MAX_READINGS_PER_SENSOR + 1];

    for(double x : separatedSensorValues) {
        x = (double) 0.00;
    }

    char sensorData[MAX_SENSOR_DATA + 1] = "";
     
    // separate different readings from the same sensor into an array of doubles
    int j = 0;
    for (int i = 0; Wire.available(); i++) { 

        byte recievedByte = Wire.read();

        if(recievedByte == ',') {
            // comma; end of individual value
            separatedSensorValues[j] = atof(sensorData);
            sensorData[0] = 0x00;

            // we want to start at character 0 next time we get here,
            // so i is set to -1 since i increments before we get here.
            i = -1;
            // increment to next separatedSensorValue
            j++;

        } else {

            // add byte to data string
            sensorData[i] = recievedByte;

        }

        if(recievedByte == '\0') {
            // end of string
            separatedSensorValues[j] = atof(sensorData);
            sensorData[0] = '\0';
            break;
        }
    }


    SensorValue sensorReturnValues[MAX_READINGS_PER_SENSOR + 1]; 

    // Initialize sensorReturnValues as a termination indicator for loops
    for(int j = 0; j <= MAX_READINGS_PER_SENSOR + 1; j++) {

        sensorReturnValues[j].type = INVALID_TYPE; 
        sensorReturnValues[j].value = (0);
        sensorReturnValues[j].timeStamp = (unsigned long) (0);

    }

    // Fill in every relevant value of sensorReturnValues with the value returned by the sensor
    for(int j = 0; this->m_readingTypes[j] != INVALID_TYPE && j < MAX_READINGS_PER_SENSOR; j++) {
        
        sensorReturnValues[j].type = (ReadingType) (m_readingTypes[j]);
        sensorReturnValues[j].value = separatedSensorValues[j];
        sensorReturnValues[j].timeStamp = (unsigned long) (( (float) millis() ) / 1000);

    }

    // Pack data into structures and return them
    int i = 0;
    for(SensorValue v : sensorReturnValues) {
        outputLocation[i] = v;
        i++;
    }

    return responseCode;
}



// Sends a character string command for a specific sensor over I2C, then returns the response code.
int Sensor_Base::sendI2CMessage(char cmd[MAX_COMMAND_LENGTH + 1]) {
    Wire.beginTransmission(this->m_address);                         
    Wire.write( cmd );                                                    
    Wire.endTransmission(true);

    delay(this->m_readDelayMS);

    Wire.requestFrom(m_address, MAX_SENSOR_DATA, 1);                                  
    int responseCode = Wire.read();
    Wire.endTransmission(true);

    return responseCode;
}



// ===== Methods for EC Subclass ====================================================================================================================================================



// Constructor for the Atlas electrical conductivity (EC) sensor;
// EC has I2C address 100, and requires a delay of 800 ms
Sensor_EC::Sensor_EC() : Sensor_Base((int)100, (unsigned long)800) {

    // populate m_readingTypes and associated m_displayNames

    this->m_readingTypes[0] = EC;
    strncpy(this->m_displayNames[0], "Electrical Conductivity", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = TDS;
    strncpy(this->m_displayNames[1], "Total Dissolved Solids", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[2] = SL;
    strncpy(this->m_displayNames[2], "Salinity", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[3] = SG;
    strncpy(this->m_displayNames[3], "Specific Gravity", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[4] = INVALID_TYPE;

}


 // enables all EC reading types to be returned from the sensor,
 // so we get the expected number of return vaues in the expected order.
int Sensor_EC::enableAllParameters() {

    #define MAX_DATA_SENT_ON_O_COMMAND (9)

    char cmd[][MAX_DATA_SENT_ON_O_COMMAND] = {
        {"O,EC,1"}, 
        {"O,TDS,1"}, 
        {"O,S,1"}, 
        {"O,SG,1"},
        {""}
        };

    for(int i = 0; i < (int)( sizeof(cmd) / sizeof(cmd[0]) ) - 1; i++) {
        
        
        int responseCode = sendI2CMessage(cmd[i]);
        
        if(responseCode != SUCCESS) {
            return responseCode;
        }
        
    }

    return SUCCESS;

}


// ===== Methods for OR Subclass ====================================================================================================================================================



// Constructor for the Atlas oxygen reduction (OR) sensor; 
// OR has I2C address 98, and requires a delay of 815 ms
Sensor_OR::Sensor_OR() : Sensor_Base((int)98, (unsigned long)815) {


    // populate m_readingTypes and associated m_displayNames

    this->m_readingTypes[0] = OR;
    strncpy(this->m_displayNames[0], "Oxygen Reduction", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}


// ===== Methods for DO Subclass ====================================================================================================================================================


// Constructor for the Atlas dissolved oxygen (DO) sensor; 
// DO has I2C address 97, and requires a delay of 575 ms
Sensor_DO::Sensor_DO() : Sensor_Base((int)97, (unsigned long)575) {


    // populate m_readingTypes and associated m_displayNames

    this->m_readingTypes[0] = DOD;
    strncpy(this->m_displayNames[0], "Dissolved Oxygen Density", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = DOP;
    strncpy(this->m_displayNames[1], "Dissolved Oxygen Percent", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}

// enables all DO reading types to be returned from the sensor,
// so we get the expected number of return vaues in the expected order.
int Sensor_DO::enableAllParameters() {

    char cmd[][MAX_DATA_SENT_ON_O_COMMAND] = {
        {"O,mg,1"}, 
        {"O,%,1"}, 
        {""}
        };

    for(int i = 0; i < (int)( sizeof(cmd) / sizeof(cmd[0]) ) - 1; i++) {

        int responseCode = sendI2CMessage(cmd[i]);

        if(responseCode != SUCCESS) {
            return responseCode;
        }

    }

    return SUCCESS;

}


// ===== Methods for PH Subclass ====================================================================================================================================================


// Constructor for the Atlas potential hydrogen (PH) sensor; 
// PH has I2C address 99, and requires a delay of 815 ms
Sensor_PH::Sensor_PH() : Sensor_Base((int)99, (unsigned long)815) {

    // populate m_readingTypes and associated m_displayNames

    this->m_readingTypes[0] = PH;
    strncpy(this->m_displayNames[0], "Potential Hydrogen", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}


// ===== Methods for TEMP Subclass ====================================================================================================================================================


// Constructor for the Atlas temperature (TEMP) sensor; 
// TEMP has I2C address 102, and requires a delay of 815 ms
Sensor_TEMP::Sensor_TEMP() : Sensor_Base((int)102, (unsigned long)815) {

    // populate m_readingTypes and associated m_displayNames

    this->m_readingTypes[0] = PH;
    strncpy(this->m_displayNames[0], "Temperature", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}