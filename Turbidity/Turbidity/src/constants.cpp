#include "constants.h"

// Define constants in the .cpp file
const uint8_t I2C_ADDRESS_DEFAULT = 0x20;
const byte Photoresistor_Pin = 2;

// Define and initialize the structs
volatile memoryMap registerMap = {
    .id = I2C_ADDRESS_DEFAULT, // Default I2C Address (0x20)
    .firmwareMajor = 0x02,     // Firmware version. Helpful for tech support.
    .firmwareMinor = 0x06,
    .X_MSB = 0,
    .X_LSB = 0,
    .temp = 0,
    .temp1 = 0,
    .temp2 = 0,
    .temp3 = 0,
    .i2cLock = 0x00,
    .i2cAddress = I2C_ADDRESS_DEFAULT,
};

memoryMap protectionMap = {
    .id = 0x00,
    .firmwareMajor = 0x00,
    .firmwareMinor = 0x00,
    .X_MSB = 0x00,
    .X_LSB = 0x00,
    .temp = 0x00,
    .temp1 = 0x00,
    .temp2 = 0x00,
    .temp3 = 0x00,
    .i2cLock = 0xFF,
    .i2cAddress = 0xFF,
};

// Define pointers to the structs
uint8_t *registerPointer = (uint8_t *)&registerMap;
uint8_t *protectionPointer = (uint8_t *)&protectionMap;

volatile byte registerNumber = 0; // Initialize the register number
