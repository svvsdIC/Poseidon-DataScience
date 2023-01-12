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

struct AtlasSensor
{
    int address;
    unsigned long readDelayMS;
    String sensorName;
};

char *readSensor(AtlasSensor sensor);
void initSensors();

#endif // #ifndef __SENSOR_H