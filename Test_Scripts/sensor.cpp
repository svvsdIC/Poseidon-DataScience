#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"


float readSensor(AtlasSensor sensor) {



    int sensorI2CAddress = sensor.address;
    unsigned long delayTime = sensor.readDelayMS;

    char cmd[] = {'r', '\0'};

    Wire.beginTransmission(sensorI2CAddress);                          // call the circuit by its ID number.
    Wire.write( cmd );                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                       //end the I2C data transmission.

    delay(delayTime); // wait the correct amount of time for the circuit to complete its instruction.

    Wire.requestFrom(sensorI2CAddress, 20, 1);                                    // call the circuit and request 20 bytes (this may be more than we need)
    int responseCode = Wire.read();               		         //the first byte is the response code, we read this separately.

    switch (responseCode)
    {                                       // switch case based on what the response code is.
    case 1:                         		//decimal 1.
        Serial.println("Success");    		//means the command was successful.
        break;                        		//exits the switch case.

    case 2:                         		//decimal 2.
        Serial.println("Failed");     		//means the command has failed.
        break;                        		//exits the switch case.

    case 254:                       		//decimal 254.
        Serial.println("Pending");    		//means the command has not yet been finished calculating.
        break;                        		//exits the switch case.

    case 255:                       		//decimal 255.
        Serial.println("No Data");    		//means there is no further data to send.
        break;                        		//exits the switch case.
    }
   
    #define MAX_SENSOR_DATA (20)
    char sensorData[MAX_SENSOR_DATA];
    
    for (int i = 0; Wire.available(); i++) {
        byte in_char = Wire.read();
        sensorData[i] = in_char;
    }

    // this has a memory leak.  maybe we should use std::string?  I have only seen people reccomending this over deallocating the memory back in main.  

    /*char *returnData = new char[MAX_SENSOR_DATA];
    strcpy(returnData, sensorData);
    return *returnData;   */      
    return atof(sensorData);
}

void initSensors() {
    
    Wire.begin();
}