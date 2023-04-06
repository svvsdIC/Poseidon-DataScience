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

struct AtlasSensor
{
    int address;
    unsigned long readDelayMS;
    enum readingType type;
};

struct SensorReading {
    readingType type;
    char timeStamp[20];
    double value;
};

int readSensor(AtlasSensor sensor/*, char * sensorData */);
void initSensors();

#endif // #ifndef __SENSOR_H