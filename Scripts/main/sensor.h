/*

File Description:

    Defines classes and structures used to read and record data from Atlas sensors.

    Header file of sensor.cpp

*/

#ifndef __SENSOR_H
#define __SENSOR_H


// Defines a type for each data point returned by a sensor.
enum ReadingType
{
    
    OR, 

    PH,

    DOD,
    DOP, 
    
    TEMP,

    EC,
    TDS, 
    SL,
    SG,

    NUM_READING_TYPES,

    INVALID_TYPE = -1
};

// Response codes returned after an I2C command
enum ResponseCodes 
{

    NO_RESPONSE = -1,
    SUCCESS = 1,
    SYNTAX = 2,
    NOT_READY = 254,
    NO_DATA = 255

};


// most bytes that can be returned by a sensor
#define MAX_SENSOR_DATA (32) 

// Maximum individual data types returned by a sensor after one read request
#define MAX_READINGS_PER_SENSOR (4)

// Max length of the text display name of a reading type
#define MAX_READING_NAME_LENGTH (32)

// Max lenghth of a command sent to a sensor
#define MAX_COMMAND_LENGTH (32)

// The header to be printed at the top of the sensor data csv file
static char SENSOR_CSV_HEADER[] = "Timestamp,Reading Type,Value";

// Associates a datapoint with its type, and the time it was measured
struct SensorValue {
    ReadingType type;
    unsigned long timeStamp;
    double value;
};


// Base class for an Atlas sensor
class Sensor_Base {
    public:
        // I2C address of sensor
        int m_address;

        // delay after command before data is available to read
        unsigned long m_readDelayMS;    

        // array of the names to display for each ReadingType this sensor returns
        char m_displayNames[MAX_READINGS_PER_SENSOR + 1][MAX_READING_NAME_LENGTH + 1];

        //Array of the ReadingTypes this sensor returns
        ReadingType m_readingTypes[MAX_READINGS_PER_SENSOR + 1]; 
                
        int read(SensorValue (&outputLocation)[MAX_READINGS_PER_SENSOR + 1]);

        // Empty virtual void to make sensors with multiple ReadingTypes
        // return all of the corresponding SensorValues (overridden in subclasses)
        virtual int enableAllParameters() {return SUCCESS;};

        Sensor_Base(int address, unsigned long readDelayMS);

    protected:
        int sendI2CMessage(char cmd[MAX_COMMAND_LENGTH + 1]);

    // TODO: add calibrate(), sleep(), status(), etc.

};



// electrical conductivity (E) sensor subclass
class Sensor_EC : public Sensor_Base {
    
    public:
        Sensor_EC();
        int enableAllParameters();

};

// oxygen (OR) reduction sensor subclass
class Sensor_OR : public Sensor_Base {
    
    public:
        Sensor_OR();

};

// potential hydrogen (PH) sensor subclass
class Sensor_PH : public Sensor_Base {
    
    public:
        Sensor_PH();

};

// dissolved oxygen (DO) sensor subclass
class Sensor_DO : public Sensor_Base {
    
    public:
        Sensor_DO();
        int enableAllParameters();


};

// temperature (TEMP) sensor subclass
class Sensor_TEMP : public Sensor_Base {
    
    public:
        Sensor_TEMP();

};



#endif // #ifndef __SENSOR_H