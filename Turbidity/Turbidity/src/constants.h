#pragma once

#include <Arduino.h>
#ifndef byte
using byte = unsigned char;
#endif

// Extern declarations for constants and variables
extern const uint8_t I2C_ADDRESS_DEFAULT;
extern const byte Photoresistor_Pin;
extern const byte Light_Pin;

// Struct definition for the register map
// Will need to add more register variables later, such as brightness
struct memoryMap
{
    byte PhotoMSB; // Reg: 0x00 - Current Joystick Horizontal Position (MSB)
    byte PhotoLSB; // Reg: 0x01 - Current Joystick Horizontal Position (LSB)
    byte i2cAddress; // Reg: 0x02 - Set I2C New Address (re-writable). Clears i2cLock.
    byte msReadDelay; // Reg: 0x03 - Milliseconds between samples
    byte numSamples;  // Reg: 0x04 - Number of samples to average
};

// Extern declarations for the register map and pointers
extern volatile memoryMap registerMap; // Specific use of the generic struct

extern uint8_t *registerPointer; // Memory manip stuff

// I don't think we use this anymore
extern volatile byte registerNumber; // Gets set when user writes an address.