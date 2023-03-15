#ifndef __SENSOR_H
#define __SENSOR_H

enum sensorNames
{
    oxygenReduction,
    ph,
    dissolvedOxygen,
    electricalConductivity,
    temperature
};

#define SENSOR_BUFFER_SIZE (32)

struct AtlasSensor
{
    int address;
    unsigned long readDelayMS;
    String sensorName;
    SensorOutput* outputDataLocation;
};

struct SensorOutput {
    char timeStamp[20];
    char sensorData[32];
};

void readSensor(AtlasSensor &sensor);
void initSensors();

#endif // #ifndef __SENSOR_H