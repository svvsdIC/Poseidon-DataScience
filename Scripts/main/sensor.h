#ifndef __SENSOR_H
#define __SENSOR_H

enum sensorType 
{
    OR, PH, DO, TEMP, EC, TDS, SL, SG, NUM_SENSOR_TYPES
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
    enum sensorType type;
};

struct SensorReading {
    sensorType type;
    char timeStamp[20];
    float value;
};

void readSensor(AtlasSensor &sensor);
void initSensors();

#endif // #ifndef __SENSOR_H