#ifndef __SENSOR_H
#define __SENSOR_H

enum readingType 
{
    // first 5 are used both for identifying the sensor type and datapoint type
    OR, 
    PH,
    DO, 
    TEMP,
    EC,
    // The rest are datapoint types only, not distinct sensors
    TDS, 
    SL,
    SG,
    NUM_SENSOR_TYPES
};

static const String measurementNames[NUM_SENSOR_TYPES]
{
    "Oxygen Reduction",             /* OR */
    "pH",                           /* PH */
    "Dissolved Oxygen",             /* DO */
    "Temperature",                  /* TEMP */
    "Electrical Conductivity",      /* EC */    
    "Total Dissolved Solids",       /* TDS */
    "Salinity"                      /* SL */
    "Specific Gravity"              /* SG */
};

#define SENSOR_BUFFER_SIZE (16)

#define MAX_READINGS_PER_SENSOR (4)

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