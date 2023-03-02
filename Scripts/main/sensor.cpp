#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"


void readSensor(AtlasSensor sensor) {



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
   
    #define MAX_SENSOR_DATA (32)
    char sensorData[MAX_SENSOR_DATA];
    
    byte in_char = 0xff;

    
    for (int i = 0; Wire.available(); i++) {
        byte in_char = Wire.read();
        sensorData[i] = in_char;
        Serial.print(in_char);
        if(in_char == 0x00) {
            break;
        }
        Serial.print(", ");
    }
    

    /*char *returnData = new char[MAX_SENSOR_DATA];
    strcpy(returnData, sensorData);
    return *returnData;   */      
    //return atof(sensorData);
    Serial.print("\n");
    Serial.println(sensorData);
    Serial.println("\n");
}

void initSensors() {
    
    Wire.begin();
    //Wire.setClock(10000); // hz
}