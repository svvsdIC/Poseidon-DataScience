#include <Arduino.h>
#include "sensor.h"



// Sensor Use cases documented here:
// https://docs.google.com/document/d/1oTBluc5CXEzSeSb-SR_KXQSh2Zgw1MtRs8IzuibEjN8

/* TODO:
[] constantly read unless told to turn off, return values when asked for
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







/*AtlasSensor sensors[] = {

    


    {98, 815, OR},
    {99, 815, PH}, 
    {97, 575, DO},
    {102, 600, TEMP},
    {100, 800, EC}
};*/



//sensorOutputData[]

#define RECIEVED_BUFFER_SIZE (32)

char serialCommand[RECIEVED_BUFFER_SIZE];
int currentByte = 0;
bool serialCommandReceived = false;

Sensor_EC obj_EC = Sensor_EC(); // filler names for testing
Sensor_DO obj_DO = Sensor_DO();


void setup() {

    Serial.begin(9600);
    Serial.println("Serial Initialized");
    initSensors();
    //delay(300);
    //Serial.println("Setup done");

    //obj_EC.enableAllParameters();
    //obj_DO.enableAllParameters();
    
}

void loop() {

    //Serial.println("loop");

    if(serialCommandReceived) {
        // follow command
        serialCommandReceived = false;
    }

    
    
    SensorValue returnedValues[MAX_READINGS_PER_SENSOR + 1];

    returnedValues[0].timeStamp = 0;
    returnedValues[0].value = 0;
    returnedValues[0].type = INVALID_TYPE;

    //Serial.println("Abooout to read DO");


    Serial.println(obj_DO.read(returnedValues));

    //Serial.println("lived through read()");
    delay(1000);


    for(int i = 0; i < 2; i++) {
        Serial.print(obj_DO.m_displayNames[i]);
        Serial.print(" measured: ");
        Serial.println(returnedValues[i].value);
    }

    delay(4000);





    returnedValues[0].timeStamp = 0;
    returnedValues[0].value = 0;
    returnedValues[0].type = INVALID_TYPE;

    //Serial.println("Abooout to read EC");

    Serial.println(obj_EC.read(returnedValues));


    for(int i = 0; i < 4; i++) {
        Serial.print(obj_EC.m_displayNames[i]);
        Serial.print(" measured: ");
        Serial.println(returnedValues[i].value);
    }

    


    delay(12000);

    /*for(int i = 0; i < MAX_SENSORS; i++) {

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
        

    }*/
    
    
    


}

void serialEvent() { // called after loop() when there is data in the buffer
    while(Serial.available()) {
        serialCommand[currentByte] = Serial.read();
        currentByte++;
    }
    serialCommandReceived = true;
}