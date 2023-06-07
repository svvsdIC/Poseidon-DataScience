#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

#define MAX_SENSOR_DATA (32)

// Reads the specified sensor, returns the status code, and fills in external string

int readSensor(AtlasSensor sensor, ReturnedSensorValues &outputLocation) {

    //Serial.print("Sensor I am trying to read: ");
    //Serial.println(sensor.type);


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

    for(double x : separatedSensorValues) {
        x = (double) 0.00;
    }

    char sensorData[MAX_SENSOR_DATA];

    int j = 0;
    for (int i = 0; Wire.available(); i++) { // separate different readings frm the same sensor into an array of doubles
        
        //Serial.print("iterator: ");
        //Serial.println(i);

        byte in_char = Wire.read();

        //Serial.print("in_char: ");
        //Serial.println(in_char);

        if(in_char == ',') {
            separatedSensorValues[j] = atof(sensorData);

            //Serial.print("de-comma'd sensor data: ");
            //Serial.println(sensorData);

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
    /*Serial.print("I processed this many comma separated values: ");
    Serial.println(j + 1);

    Serial.println("All of the values I have processed are listed below:");
    for(double x : separatedSensorValues) {
        Serial.print("Value: ");
        Serial.println(x);
    }*/



    SensorValue sensorReturnValues[MAX_READINGS_PER_SENSOR + 1]; 

    for(int j = 0; j <= MAX_READINGS_PER_SENSOR + 1; j++) {
        sensorReturnValues[j].type = (readingType) (-1); 
        sensorReturnValues[j].value = (0);
        sensorReturnValues[j].timeStamp = (unsigned long) (0);
    }

    switch(sensor.type) { 
        case(EC):
            //Serial.println("I think this is an EC sensor.");
            #define EC_READINGS (4)
            for(int j = 0; j < EC_READINGS; j++) {
                sensorReturnValues[j].type = (readingType) (sensor.type + j); // TODO: change to more robust system
                sensorReturnValues[j].value = separatedSensorValues[j];
                sensorReturnValues[j].timeStamp = (unsigned long) (( (float) millis() ) / 100); // TODO: change this format later
            }

            break;
        /*case(DO):

            break;*/
        default: // only one reading
            //Serial.println("I think this is NOT an EC sensor.");
            sensorReturnValues[0].type = sensor.type;
            //sensorReading.timeStamp = ; // set timestamp here
            sensorReturnValues[0].value = separatedSensorValues[0];
            sensorReturnValues[0].timeStamp = (unsigned long) (( (float) millis() ) / 100);
            break;
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