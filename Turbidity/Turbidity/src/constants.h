#pragma once

#include <Arduino.h>

// Enum for EEPROM locations
enum eepromLocations
{
    LOCATION_I2C_ADDRESS = 0, // Device's address
    LOCATION_FIRMWARE_MAJOR,
    LOCATION_FIRMWARE_MINOR,
    // Add more EEPROM locations here if needed
};

// Extern declarations for constants and variables
extern const uint8_t I2C_ADDRESS_DEFAULT;
extern const byte Photoresistor_Pin;

// Struct definition for the register map
struct memoryMap
{
    byte id;            // Reg: 0x00 - Default I2C Address
    byte firmwareMajor; // Reg: 0x01 - Firmware Number
    byte firmwareMinor; // Reg: 0x02 - Firmware Number
    byte X_MSB;         // Reg: 0x03 - Current Joystick Horizontal Position (MSB)
    byte X_LSB;         // Reg: 0x04 - Current Joystick Horizontal Position (LSB)
    byte temp;
    byte temp1;
    byte temp2;
    byte temp3;
    byte i2cLock;    // Reg: 0x09 - Must be changed to 0x13 before I2C address can be changed.
    byte i2cAddress; // Reg: 0x0A - Set I2C New Address (re-writable). Clears i2cLock.
};

// Extern declarations for the register map and pointers
extern volatile memoryMap registerMap;
extern memoryMap protectionMap;

extern uint8_t *registerPointer;
extern uint8_t *protectionPointer;

extern volatile byte registerNumber; // Gets set when user writes an address.