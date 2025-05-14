#pragma once

#include <Arduino.h>

// Enum for EEPROM locations
enum eepromLocations
{
    LOCATION_I2C_ADDRESS = 0, // Device's address
    // LOCATION_FIRMWARE_MAJOR,
    // LOCATION_FIRMWARE_MINOR,
    //  Add more EEPROM locations here if needed
};

// Extern declarations for constants and variables
extern const uint8_t I2C_ADDRESS_DEFAULT;
extern const byte Photoresistor_Pin;

// Struct definition for the register map
struct memoryMap
{
    // byte id; // Reg: 0x00 - Default I2C Address
    //  byte firmwareMajor; // Reg: 0x01 - Firmware Number
    //  byte firmwareMinor; // Reg: 0x02 - Firmware Number
    byte PhotoMSB; // Reg: 0x00 - Current Joystick Horizontal Position (MSB)
    byte PhotoLSB; // Reg: 0x01 - Current Joystick Horizontal Position (LSB)
    // byte i2cLock;    // Reg: 0x03 - Must be changed to 0x13 before I2C address can be changed.
    byte i2cAddress; // Reg: 0x02 - Set I2C New Address (re-writable). Clears i2cLock.
};

// Extern declarations for the register map and pointers
extern volatile memoryMap registerMap;

extern uint8_t *registerPointer;

extern volatile byte registerNumber; // Gets set when user writes an address.