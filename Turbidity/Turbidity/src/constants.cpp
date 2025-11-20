#include "constants.h"

// Define constants in the .cpp file
const uint8_t I2C_ADDRESS_DEFAULT = 0x20;
const byte Photoresistor_Pin = 2; // Will be 3 in the final version
const byte Light_Pin = 3;         // Will be 4 in the final version

/*
Final pinmap:
1. Reset
2. Unused
3. Photoresistor
4. Ground
5. SDA
6. Unused
7. SCL
8. Power (5V)
*/

// Define and initialize the structs
volatile memoryMap registerMap = {
    //.id = I2C_ADDRESS_DEFAULT, // Default I2C Address (0x20)
    //.firmwareMajor = 0x02,     // Firmware version. Helpful for tech support.
    //.firmwareMinor = 0x06,
    //.Photo = 0, // Current photoresistor value (10-bit ADC value)
    .PhotoMSB = 0,
    .PhotoLSB = 0,
    //.i2cLock = 0x00,
    .i2cAddress = I2C_ADDRESS_DEFAULT,
};

// Define pointers to the structs
uint8_t *registerPointer = (uint8_t *) &registerMap;

volatile byte registerNumber = 0; // Initialize the register number
