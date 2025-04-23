/*
  https://www.sparkfun.com/products/15168
  To install support for ATtiny84 in Arduino IDE: https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  This core is installed from the Board Manager menu
  If you have Dave Mellis' ATtiny installed you may need to remove it from \Users\xx\AppData\Local\Arduino15\Packages
  To support 400kHz I2C communication reliably ATtiny84 needs to run at 8MHz. This requires user to
  click on 'Burn Bootloader' before code is loaded.
*/

#include <Wire.h>
#include <EEPROM.h>
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

#include "constants.h"
#include "interrupts.h"

class i2cDevice
{
public:
    i2cDevice();
    ~i2cDevice();
    void AssignOnReceive(void (*func)());
    void AssignOnRequest(void (*func)());
};

void readI2CAddress(void);

void setup(void)
{
    // Pin Definitions
    pinMode(Photoresistor_Pin, INPUT); // No pull-up. External 10k

    readI2CAddress(); // Load all system settings from EEPROM
    startI2C();       // Determine the I2C address we should be using and begin listening on I2C bus
}

void loop(void)
{
    // Sleep until interrupt
    // set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode(); // Stop everything and go to sleep. Wake up from Button interrupts.
}

// Reads the current system settings from EEPROM
// If anything looks weird, reset setting to default value
void readI2CAddress(void)
{
    // Read what I2C address we should use
    registerMap.i2cAddress = EEPROM.read(LOCATION_I2C_ADDRESS);

    // Error check I2C address we read from EEPROM
    if (registerMap.i2cAddress < 0x08 || registerMap.i2cAddress > 0x77)
    {
        // User has set the address out of range
        // Go back to defaults
        registerMap.i2cAddress = I2C_ADDRESS_DEFAULT;
        EEPROM.write(LOCATION_I2C_ADDRESS, registerMap.i2cAddress);
    }
}
