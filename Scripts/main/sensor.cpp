#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

#define MAX_SENSOR_DATA (32)



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

Sensor_EC::Sensor_EC() : Sensor_Base((int)100, (unsigned long)800) {

    this->m_readingTypes[0] = EC;
    strncpy(this->m_displayNames[0], "Electrical Conductivity\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = TDS;
    strncpy(this->m_displayNames[1], "Total Dissolved Solids\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[2] = SL;
    strncpy(this->m_displayNames[2], "Salinity\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[3] = SG;
    strncpy(this->m_displayNames[3], "Specific Gravity\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[4] = INVALID_TYPE;

}

Sensor_OR::Sensor_OR() : Sensor_Base((int)98, (unsigned long)815) {

    this->m_readingTypes[0] = OR;
    strncpy(this->m_displayNames[0], "Oxygen Reduction\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}

Sensor_DO::Sensor_DO() : Sensor_Base((int)97, (unsigned long)575) {

    this->m_readingTypes[0] = DOD;
    strncpy(this->m_displayNames[0], "Dissolved Oxygen Density\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = DOP;
    strncpy(this->m_displayNames[1], "Dissolved Oxygen Percent\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}

Sensor_PH::Sensor_PH() : Sensor_Base((int)99, (unsigned long)815) {

    this->m_readingTypes[0] = PH;
    strncpy(this->m_displayNames[0], "Potential Hydrogen\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}

Sensor_TEMP::Sensor_TEMP() : Sensor_Base((int)102, (unsigned long)815) {

    this->m_readingTypes[0] = PH;
    strncpy(this->m_displayNames[0], "Temperature\0", MAX_READING_NAME_LENGTH);

    this->m_readingTypes[1] = INVALID_TYPE;
    this->m_readingTypes[2] = INVALID_TYPE;
    this->m_readingTypes[3] = INVALID_TYPE;
    this->m_readingTypes[4] = INVALID_TYPE;

}

// Reads the specified sensor, returns the status code, and fills in external string

int Sensor_Base::read(SensorValue (&outputLocation)[MAX_READINGS_PER_SENSOR + 1]) {

    //Serial.print("Sensor I am trying to read: ");
    //Serial.println(this->m_displayNames[0]);



    char cmd[] = {'r', '\0'};



    Wire.beginTransmission(this->m_address); 


    Wire.write( cmd );                                                    
    Wire.endTransmission(true);  

    delay(this->m_readDelayMS); 
    //Serial.println("lived to pos. 0");

    //delay(1000);

    Wire.requestFrom(this->m_address, MAX_SENSOR_DATA + 1, 1);  
    //Serial.println("lived to pos. 1");                       
    //delay(1000);         
    int responseCode = (int) Wire.read();   // the first byte is the response code
    //Serial.println("lived to pos. 2");
    //Serial.print("Read responseCode: ");
    //Serial.println(responseCode);
    //delay(1000);
    
       
    
    double separatedSensorValues[MAX_READINGS_PER_SENSOR + 1];

    for(double x : separatedSensorValues) {
        x = (double) 0.00;
    }

    char sensorData[MAX_SENSOR_DATA + 1];

    sensorData[0] = '\0';
     

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

    for(int j = 0; this->m_readingTypes[j] != INVALID_TYPE && j < MAX_READINGS_PER_SENSOR; j++) {
        

        sensorReturnValues[j].type = (ReadingType) (m_readingTypes[j]); // TODO: change to more robust system
        sensorReturnValues[j].value = separatedSensorValues[j];
        sensorReturnValues[j].timeStamp = (unsigned long) (( (float) millis() ) / 1000);
        //Serial.print("");
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
        //Serial.println(v.type);
        i++;
    }



    //Serial.println("Line before return in read()");
    //delay(1000);



    return responseCode;
}

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


void Sensor_EC::enableAllParameters() { // enables all EC reading types to be returned from the sensor, so we get the expected number of return vaues.

    #define MAX_DATA_SENT_O (9)

    char cmd[][MAX_DATA_SENT_O] = {
        {"O,EC,1\0"}, 
        {"O,TDS,1\0"}, 
        {"O,S,1\0"}, 
        {"O,SG,1\0"},
        {"/0"}
        };

    for(int i = 0; i < (int)( sizeof(cmd) / sizeof(cmd[0]) ) - 1; i++) {
        //char cmd[] = ;

        //Serial.println(cmd[i]);


        int responseCode = sendI2CMessage(cmd[i]);

        Wire.requestFrom(m_address, MAX_SENSOR_DATA, 1);                                  
        Wire.endTransmission(true);
        

    }

    //return 1;

}



void Sensor_DO::enableAllParameters() {

    #define MAX_DATA_SENT_O (9)

    char cmd[][MAX_DATA_SENT_O] = {
        {"O,mg,1\0"}, 
        {"O,%,1\0"}, 
        {"\0"}
        };

    for(int i = 0; i < (int)( sizeof(cmd) / sizeof(cmd[0]) ) - 1; i++) {
        //char cmd[] = ;

        //Serial.println(cmd[i]);


        int responseCode = sendI2CMessage(cmd[i]);

        Wire.requestFrom(m_address, MAX_SENSOR_DATA, 1);                                  
        Wire.endTransmission(true);

    }

    //return 1;

}

// classname objectname = classname(parameter); 




void initSensors() {
    
    Wire.begin();


}