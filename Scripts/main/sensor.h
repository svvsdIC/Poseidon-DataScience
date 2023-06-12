#ifndef __SENSOR_H
#define __SENSOR_H

enum ReadingType  // group all child readings under their corresponding parent reading, in order.
{
    
    OR, 

    PH,

    DO, 
    
    TEMP,

    EC,
    TDS, 
    SL,
    SG,

    NUM_READING_TYPES,

    INVALID_TYPE = -1
};

/*static const String measurementNames[NUM_READING_TYPES]
{
    "Oxygen Reduction",             
    "pH",                           
    "Dissolved Oxygen",             
    "Temperature",                  
    "Electrical Conductivity",          
    "Total Dissolved Solids",       
    "Salinity",                      
    "Specific Gravity"              
};*/

#define SENSOR_BUFFER_SIZE (32)

#define MAX_READINGS_PER_SENSOR (4)

#define MAX_READING_NAME_LENGTH (32)

#define MAX_SENSORS (5)





/*struct AtlasSensor
{
    int address;
    unsigned long readDelayMS;
    enum readingType type;
};*/

struct SensorValue {
    ReadingType type;
    unsigned long timeStamp;
    double value;
};


class AtlasSensor {
    public:
        int m_address;
        unsigned long m_readDelayMS;

        char m_displayNames[MAX_READINGS_PER_SENSOR + 1][MAX_READING_NAME_LENGTH + 1];
        ReadingType m_readingTypes[MAX_READINGS_PER_SENSOR + 1]; 
        
        // m_returnValues[MAX_READINGS_PER_SENSOR + 1];


        int read(SensorValue (&outputLocation)[MAX_READINGS_PER_SENSOR + 1]);

        AtlasSensor(int address, unsigned long readDelayMS);

    // add calibration, sleep, status, etc.


};




class EC_Sensor : public AtlasSensor {
    

    public:
        EC_Sensor();

};







void initSensors();

#endif // #ifndef __SENSOR_H