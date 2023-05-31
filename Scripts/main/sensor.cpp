#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

#define MAX_SENSOR_DATA (32)

// Reads the specified sensor, returns the status code, and fills in external string

int readSensor(AtlasSensor sensor, ReturnedSensorValues &outputLocation) {



    int sensorI2CAddress = sensor.address;
    unsigned long delayTime = sensor.readDelayMS;

    char cmd[] = {'r', '\0'};

    Wire.beginTransmission(sensorI2CAddress);                          // call the circuit by its ID number.
    Wire.write( cmd );                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                       //end the I2C data transmission.

    delay(delayTime); // wait the correct amount of time for the circuit to complete its instruction.

    Wire.requestFrom(sensorI2CAddress, MAX_SENSOR_DATA, 1);                                    // call the circuit and request 32 bytes
    int responseCode = Wire.read();               		         //the first byte is the response code, we read this separately.

       
    
    double separatedSensorValues[MAX_READINGS_PER_SENSOR + 1];

    char sensorData[MAX_SENSOR_DATA];

    int j = 0;
    for (int i = 0; Wire.available(); i++) { // separate different readings frm the same sensor into an array of doubles

        byte in_char = Wire.read();

        if(in_char == ',') {
            separatedSensorValues[j] = atof(sensorData);
            sensorData[0] = 0x00;
            j++;
        } else {
            sensorData[i] = in_char;
        }

        if(in_char == 0x00) {
            break;
        }
    }



    SensorValue sensorReturnValues[MAX_READINGS_PER_SENSOR + 1]; 

    switch(sensor.type) { // sensor.type is the ID of the sensor we read
        case(EC):
            #define EC_READINGS (4)
            for(int i = 0; i < EC_READINGS; i++) {
                sensorReturnValues[i].type = (readingType) (sensor.type + i); // TODO: change to more robust system
                sensorReturnValues[i].value = separatedSensorValues[i];
                sensorReturnValues[i].timeStamp = millis(); // change this later
            }
            break;
        /*case(DO):

            break;*/
        default: // only one reading
            sensorReturnValues[0].type = sensor.type;
            //sensorReading.timeStamp = ; // set timestamp here
            sensorReturnValues[0].value = separatedSensorValues[0];
            sensorReturnValues[0].timeStamp = millis();
            break;
    }

    // pack data into structures and return them

    ReturnedSensorValues finalReturnValues;


    for(int i = 0; i <= MAX_READINGS_PER_SENSOR + 1; i++) {
        finalReturnValues.values[i] = sensorReturnValues[i];
    }

    outputLocation = finalReturnValues;

    return responseCode;
}

void initSensors() {
    
    Wire.begin();
    
}