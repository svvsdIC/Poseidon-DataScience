# Poseidon-DataScience


Software to run an arbitrary number of sensor control boards to measure and log water properties, with support for sensors that return multiple values via an I2C bus.  Measurements are packaged with their timestamp and type.  This software is intended for use with an Adafruit M0 Adalogger, and uses the built in SD card to record values in .csv format.

The file main.ino contains the process for requesting and recording data values from the sensors.  Additionally, it contains the logic for recieving and executing commands from an external source. It also handles program startup and initialization.

The files sensor.h and sensor.cpp contain the sensor_base class and its subclasses.  Each subclass defines a different type of sensor, which are instantiated in main.ino.  Most sensors are Atlas brand. Datasheets are at https://atlas-scientific.com. 

Files eventlogger.cpp and .h define the Event_Logger class, whose methods govern the recording of an event and error log on the M0's microSD card.

The serialcommand .cpp and .h files define the class Serial_Command, which holds the logic determining the interpretation of commands recieved over the serial line.

The utilities.cpp and .h files contain miscellaneous functions including text formating and file manipulation.