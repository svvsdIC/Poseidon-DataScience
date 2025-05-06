#include <Wire.h>
#include <EEPROM.h>
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

#include "constants.h"
#include "interrupts.h"

// Reads the current system settings from i2c address
// void readI2CAddress(void)
// {
//     // Read what I2C address we should use
//     registerMap.i2cAddress = I2C_ADDRESS_DEFAULT;
// }

void setup(void)
{
    // Pin Definitions
    pinMode(Photoresistor_Pin, INPUT); // No pull-up. External 10k

    //readI2CAddress(); // Load all system settings from EEPROM
    startI2C();       // Determine the I2C address we should be using and begin listening on I2C bus
}

void loop(void)
{
    // Sleep until interrupt
    // set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode(); // Stop everything and go to sleep. Wake up from Button interrupts.
}