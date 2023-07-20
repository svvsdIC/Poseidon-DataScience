/*

    File Description:

        Contains miscellaneous functions needed in main.ino that do not pertain
        directly to Atlas sensors.

*/

#include <Arduino.h>
#include "utilities.h"



//Takes an input in seconds and formats it into the form "hh:mm:ss"
// then returns it to outputLocation
void formatTime(unsigned long seconds, char (&outputLocation)[MAX_TIME_CHARS + 1]) {

    unsigned int hours = (int) ( seconds / 3600 );
    seconds -= (hours * 3600);
    hours %= 24;

    unsigned int minutes = (int) ( seconds / 60 );
    seconds -= minutes * 60;

    sprintf(outputLocation, "%0u:%02u:%02u", hours, minutes, seconds);

}