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
    char outputData[SENSOR_BUFFER_SIZE];
};

void readSensor(AtlasSensor &sensor);
void initSensors();

#endif // #ifndef __SENSOR_H