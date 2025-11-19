#include <Wire.h>
#include <EEPROM.h>
// Don't need these rn, but will need to create a "sleep" command to save energy
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

#include "constants.h"
#include "interrupts.h"

void setup(void)
{
    // Pin Definitions
    pinMode(Photoresistor_Pin, INPUT); // No pull-up. (External 10k) - What?
    pinMode(Light_Pin, OUTPUT);        // Light Pin

    // Initialize the i2c protocol and necessary requirements.
    // This listens all the time, but also assigns the onRecieve and onRequest callbacks
    startI2C(); // Determine the I2C address we should be using and begin listening on I2C bus
}

void loop(void)
{
    // Sleep until interrupt
    sleep_mode(); // Stop everything and go to sleep. Wake up from Button interrupts.
}