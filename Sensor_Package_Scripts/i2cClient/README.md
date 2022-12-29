## i2cClient.ino

This is an i2C client that listens to all i2c addresses, and can be tailored
to respond to requests differently depending upon the i2c address of the transaction.

It can serve as an emulator for a collection of sensors, or other i2C devices.

This is an arduino sketch that runs on many arm processors, including ARDUINO-UNO and ARDUINO-MEGA-2560.

It modifies twi.c to use the TWAMR (address mask register) of the processor to mask for all i2c addresses, not just a specific one.

It modifies twi.c so that on a request for data it captures the i2c address 
used by the i2c controller, and makes it available to a caller via the
"extern uint8_t twi_TWAR;" variable. Thus it is available to the i2cClient.ino function registered with the Wire.onrequest method.

It has been setup to respond to i2c addresses of several ATLAS Sensors, with simulated values when asked to return sensor values.

