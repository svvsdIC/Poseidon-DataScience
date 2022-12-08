#include <Arduino.h>

#include "sensor.h"

#define address 97 // default I2C ID number for EZO DO Circuit.

// TODO: add sensor class

// TODO: globals are bad, restructure

char computerdata[20];           // we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte received_from_computer = 0; // we need to know how many characters have been received.
byte serial_event = 0;           // a flag to signal when data has been received from the pc/mac/other.
byte code = 0;                   // used to hold the I2C response code.
char do_data[20];                // we make a 20 byte character array to hold incoming data from the D.O. circuit.
byte in_char = 0;                // used as a 1 byte buffer to store inbound bytes from the D.O. Circuit.
byte i = 0;                      // counter used for DO_data array.
int time_ = 575;                 // used to change the delay needed depending on the command sent to the EZO Class DO Circuit.

char *DO;  // char pointer used in string parsing.
char *sat; // char pointer used in string parsing.

float DO_float;  // float var used to hold the float value of the D.O.
float sat_float; // float var used to hold the float value of the percent saturation.

void setup() {
    Serial.begin(9600);
    initSensors();
}

void serialEvent() {                                                                         // this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.
    received_from_computer = Serial.readBytesUntil(13, computerdata, 20); // we read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received.
    computerdata[received_from_computer] = 0;                             // stop the buffer from transmitting leftovers or garbage.
    serial_event = true;                                                  // set the serial event flag.
}

void loop() { // the main loop.
    if (serial_event == true) { // if a command was sent to the EZO device.
        for (i = 0; i <= received_from_computer; i++)
        {                                               // set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
            computerdata[i] = tolower(computerdata[i]);
        }
        i = 0; // reset i, we will need it later
        if (computerdata[0] == 'c' || computerdata[0] == 'r')
            time_ = 575; // if a command has been sent to calibrate or take a reading we wait 575ms so that the circuit has time to take the reading.
        else
            time_ = 250; // if any other command has been sent we wait only 250ms.

        readSensor(address, time_);
        
        serial_event = false;
    }
}


void string_pars() { // this function will break up the CSV string into its 2 individual parts. DO|SAT|
// this is done using the C command “strtok”.

    DO = strtok(do_data, ","); // let's parse the string at each comma.
    sat = strtok(NULL, ",");   // let's pars the string at each comma.

    Serial.print("DO:"); // we now print each value we parsed separately.
    Serial.println(DO);  // this is the D.O. value.

    Serial.print("SAT:"); // we now print each value we parsed separately.
    Serial.println(sat);  // this is the percent saturation.
    Serial.println();
}