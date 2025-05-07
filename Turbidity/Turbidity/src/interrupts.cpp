#include <EEPROM.h>
#include <Wire.h>
#include "constants.h"
#include "interrupts.h"

// Begin listening on I2C bus as I2C slave using the global variable setting_i2c_address
void startI2C()
{
    Wire.end(); // Before we can change addresses we need to stop

    Wire.begin(registerMap.i2cAddress); // Start I2C and answer calls using address from EEPROM

    // The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

// Called every time the joystick is moved
void updateTurbidity()
{
    // Bring up ADC
    ADCSRA |= (1 << ADEN);

    registerMap.Photo = analogRead(Photoresistor_Pin);

    // Convert int value to a 16-bit integer by bitshitting it left by 6.
    // Example-
    // DEC:           630
    // BIN:           1001110110
    // Bitshift by 6: 1001110110000000
    //uint16_t photo16 = photo << 6;

    // Split the bitshifted word into the MSB and LSB parts
    // Continuing from the example above...
    // DEC:            630
    // BIN:            10011101   10
    // Bitshift by 6:  10011101   10000000
    // Split:          10011101 | 10000000

    // Convert the 16-bit integer to a byte array
    // byte *bytePointer;
    // bytePointer = (byte *)&photo16;

    // registerMap.PhotoMSB = bytePointer[1];
    // registerMap.PhotoLSB = bytePointer[0];

    // Shut off ADC
    ADCSRA &= ~(1 << ADEN);
}

// If the current setting is different from that in EEPROM, update EEPROM
// void recordI2CAddress(void)
// {
//     // I2C address is byte
//     byte i2cAddr;

//     // Error check the current I2C address
//     if (registerMap.i2cAddress < 0x08 || registerMap.i2cAddress > 0x77)
//     {
//         // User has set the address out of range
//         // Go back to defaults
//         registerMap.i2cAddress = I2C_ADDRESS_DEFAULT;
//     }

//     // Read the value currently in EEPROM. If it's different from the memory map then record the memory map value to EEPROM.
//     EEPROM.get(LOCATION_I2C_ADDRESS, i2cAddr);
//     if (i2cAddr != registerMap.i2cAddress && registerMap.i2cLock == 0x13)
//     {
//         registerMap.i2cLock = 0x00;
//         EEPROM.write(LOCATION_I2C_ADDRESS, registerMap.i2cAddress);
//         startI2C(); // Determine the I2C address we should be using and begin listening on I2C bus
//     }
// }

// When Qwiic Joystick receives data bytes from Master, this function is called as an interrupt
//(Serves rewritable I2C address)
void receiveEvent(int numberOfBytesReceived)
{
    registerNumber = Wire.read(); // Get the memory map offset from the user

    // Begin recording the following incoming bytes to the temp memory map
    // starting at the registerNumber (the first byte received)
    for (byte x = 0; x < numberOfBytesReceived - 1; x++)
    {
        byte temp = Wire.read(); // We might record it, we might throw it away

        if ((x + registerNumber) < sizeof(memoryMap))
        {
            // Clense the incoming byte against the read only protected bits
            // Store the result into the register map
            *(registerPointer + registerNumber + x) = temp;
        }
    }
    // recordI2CAddress();
}

// Respond to GET commands
// When Qwiic Joystick gets a request for data from the user, this function is called as an interrupt
// The interrupt will respond with bytes starting from the last byte the user sent to us
// While we are sending bytes we may have to do some calculations
void requestEvent()
{
    updateTurbidity();

    // This will write the entire contents of the register map struct starting from
    // the register the user requested, and when it reaches the end the master
    // will read 0xFFs.
    //Wire.write((registerPointer + registerNumber), sizeof(memoryMap) - registerNumber);
    uint8_t msb = (registerMap.Photo >> 8) & 0xFF; // Most Significant Byte
    uint8_t lsb = registerMap.Photo & 0xFF;        // Least Significant Byte

    // Send the two bytes
    Wire.write(msb);
    Wire.write(lsb);
}