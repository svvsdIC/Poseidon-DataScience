#include <Arduino.h>
#include "sensor.h"



// Sensor Use cases documented here:
// https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

/* TODO:
[] CONVERT TO CLASS STRUCTURE
[] Hours/Minutes/Seconds since start
[] low power mode - deep sleep
[] detect sensor error condition
[] onboard logging
    [] look at built-in sensor storage
[] stream live data
[] instruct single read
[] Current/voltage measurement
[] Bluetooth connection
[] Take readings on command
[] Measure with set delay
[] Detect water depth
[] Data Radio (one measure/hour)
[] Detect Error conditions
[] Local data storage
*/

#define SENSOR_STORAGE_BUFFER_SIZE (256)







AtlasSensor sensors[] = {

    /*
    {address, readDelayMS, sensorName}

    address - I2C bus adress
    readDelayMS - duration of read delay in milliseconds
    sensorType - type of sensor
    */

    {98, 815, OR},/* [status][ascii encoded signed float mV][null] */
    {99, 815, PH}, /* [status][ascii encoded floating point pH][null] */
    {97, 575, DO}, /* [status][ascii encoded float mg / L and %][null] */
    {102, 600, TEMP},  /* [status][ascii encoded float (3 decimals) celsius][null] */
    {100, 800, EC} /* [status][ascii encoded floats μS / cm, val2, val3, val4][null] */
};



//sensorOutputData[]

#define RECIEVED_BUFFER_SIZE (32)

char serialCommand[RECIEVED_BUFFER_SIZE];
int currentByte = 0;
bool serialCommandReceived = false;


void setup() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
}

void loop() {

    if(serialCommandReceived) {
        // follow command
        serialCommandReceived = false;
    }

    for(int i = 0; i < MAX_SENSORS; i++) {

        ReturnedSensorValues returnedSensorValues;

        Serial.print("Read Attempt on ");
        Serial.println(measurementNames[i]);

        //Serial.println(readSensor(sensors[i], returnedSensorValues)); // returns -1
        int responseCode = readSensor(sensors[i], returnedSensorValues);
        switch (responseCode)
        {                                       
        case 1:     
                            		//command was successful.
            Serial.println("Success");

            for(int ii = 0; (returnedSensorValues.values[ii].timeStamp != 0) && (returnedSensorValues.values[ii].type != -1); ii++) {
                Serial.print("At time: ");
                Serial.print(returnedSensorValues.values[ii].timeStamp);
                Serial.print(" cs,  ");
                Serial.print(measurementNames[(int) returnedSensorValues.values[ii].type]);    		
                Serial.print(" measured: ");
                Serial.print(returnedSensorValues.values[ii].value);
                Serial.println("\n");
            }

            break;                   		
        case 2:                         		
            Serial.println("Read Command Failed\n");     		//command has failed, probably syntax
            break;                        		

        case 254:                       	
            Serial.println("Read Pending\n");    		//command has not yet been finished calculating.
            break;        

        case 255:                       		
            Serial.println("No Data Recieved\n");    		//there is no further data to send.
            break;
                        		
        case -1:
            Serial.println("No Connection From Sensor\n");
            break;
        default:
            Serial.print("ERROR: Invalid Response Code: ");    		// Weird other response code
            Serial.println(responseCode);         

            break;    
        }
        #define DELAY_BETWEEN_SENSOR_READS (2000)

        delay(DELAY_BETWEEN_SENSOR_READS - sensors[i].readDelayMS);
        
    }


    delay(10000);


}

void serialEvent() { // called after loop() when there is data in the buffer
    while(Serial.available()) {
        serialCommand[currentByte] = Serial.read();
        currentByte++;
    }
    serialCommandReceived = true;
}