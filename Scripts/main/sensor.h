#ifndef __SENSOR_H
#define __SENSOR_H


enum readingType  // group all child readings under their corresponding parent reading, in order.
{
    
    OR, 

    PH,

    DO, 

    TEMP,

    EC,
    TDS, 
    SL,
    SG,

    NUM_READING_TYPES
};

static const String measurementNames[NUM_READING_TYPES]
{
    "Oxygen Reduction",             /* OR */
    "pH",                           /* PH */
    "Dissolved Oxygen",             /* DO */
    "Temperature",                  /* TEMP */
    "Electrical Conductivity",      /* EC */    
    "Total Dissolved Solids",       /* TDS */
    "Salinity",                      /* SL */
    "Specific Gravity"              /* SG */
};

#define SENSOR_BUFFER_SIZE (16)

#define MAX_READINGS_PER_SENSOR (4)

#define MAX_SENSORS (5)

struct AtlasSensor
{
    int address;
    unsigned long readDelayMS;
    enum readingType type;
};

struct SensorValue {
    readingType type;
    unsigned long timeStamp;
    double value;
};

struct ReturnedSensorValues {
    SensorValue values[MAX_READINGS_PER_SENSOR + 1];
};

int readSensor(AtlasSensor sensor, ReturnedSensorValues &outputLocation);
void initSensors();

#endif // #ifndef __SENSOR_H