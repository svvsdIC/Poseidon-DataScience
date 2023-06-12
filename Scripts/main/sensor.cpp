#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

#define MAX_SENSOR_DATA (32)



AtlasSensor::AtlasSensor(int address, unsigned long readDelayMS) {

    m_address = address;
    m_readDelayMS = readDelayMS;

    for(ReadingType x : m_readingTypes) {
        x = (ReadingType) INVALID_TYPE;
    }
    
    for(int i = 0; i < MAX_READINGS_PER_SENSOR; i++) {
        this->m_displayNames[i][0] = 0x00;
    }

    
        
}

EC_Sensor::EC_Sensor() : AtlasSensor((int)100, (unsigned long)800) {

    this->m_readingTypes[0] = EC;
    strncpy(this->m_displayNames[0], "Electrical Conductivity\0", MAX_READING_NAME_LENGTH);


    this->m_readingTypes[1] = TDS;
    strncpy(this->m_displayNames[1], "Total Dissolved Solids\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[2] = SL;
    strncpy(this->m_displayNames[2], "Salinity\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[3] = SG;
    strncpy(this->m_displayNames[3], "Specific Gravity\0", MAX_READING_NAME_LENGTH);

}



// Reads the specified sensor, returns the status code, and fills in external string

int AtlasSensor::read(SensorValue (&outputLocation)[MAX_READINGS_PER_SENSOR + 1]) {

    //Serial.print("Sensor I am trying to read: ");
    //Serial.println(sensor.type);


    int sensorI2CAddress = m_address;
    unsigned long delayTime = m_readDelayMS;

    char cmd[] = {'r', '\0'};

    Wire.beginTransmission(sensorI2CAddress);                          // call the circuit by its ID number.
    Wire.write( cmd );                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                       //end the I2C data transmission.

    delay(delayTime); // wait the correct amount of time for the circuit to complete its instruction.

    Wire.requestFrom(sensorI2CAddress, MAX_SENSOR_DATA, 1);                                    // call the circuit and request 32 bytes
    int responseCode = Wire.read();               		         //the first byte is the response code, we read this separately.

       
    
    double separatedSensorValues[MAX_READINGS_PER_SENSOR + 1];

    for(double x : separatedSensorValues) {
        x = (double) 0.00;
    }

    char sensorData[MAX_SENSOR_DATA + 1];

    sensorData[MAX_SENSOR_DATA + 1] = '\0';

    int j = 0;
    for (int i = 0; Wire.available(); i++) { // separate different readings frm the same sensor into an array of doubles

        byte in_char = Wire.read();


        if(in_char == ',') {
            separatedSensorValues[j] = atof(sensorData);



            sensorData[0] = 0x00;
            i = -1; // since i indexes before we get back, we want to start at character 0 next time
            j++;
        } else {
            sensorData[i] = in_char;
        }

        if(in_char == 0x00) {
            separatedSensorValues[j] = atof(sensorData);
            sensorData[0] = 0x00;
            j++;    
            break;
        }
    }


    SensorValue sensorReturnValues[MAX_READINGS_PER_SENSOR + 1]; 

    for(int j = 0; j <= MAX_READINGS_PER_SENSOR + 1; j++) {
        sensorReturnValues[j].type = INVALID_TYPE; 
        sensorReturnValues[j].value = (0);
        sensorReturnValues[j].timeStamp = (unsigned long) (0);
    }


            //Serial.println("I think this is an EC sensor.");
    for(int j = 0; m_readingTypes[j] != INVALID_TYPE; j++) {
        sensorReturnValues[j].type = (ReadingType) (m_readingTypes[j]); // TODO: change to more robust system
        sensorReturnValues[j].value = separatedSensorValues[j];
        sensorReturnValues[j].timeStamp = (unsigned long) (( (float) millis() ) / 1000);
    }
/*
    for(auto x : sensorReturnValues) {
        if(x.type != -1) {
            Serial.print("Return Value: ");
            Serial.print(x.value);
            Serial.print(";  Type: ");
            Serial.print(x.type);
            Serial.print(";  TimeStamp: ");
            Serial.println(x.timeStamp);
        }
    }
*/
    // pack data into structures and return them


    int i = 0;
    for(SensorValue v : sensorReturnValues) {
        outputLocation[i] = v;
        i++;
    }


    return responseCode;
}


// classname objectname = classname(parameter); 




void initSensors() {
    
    Wire.begin();


}