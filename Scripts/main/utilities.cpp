/*

    File Description:

        Contains miscellaneous functions needed in main.ino that do not pertain
        directly to Atlas sensors.

*/

#include <Arduino.h>
#include "utilities.h"



// Takes an input in seconds and formats it into the form "hh:mm:ss"
// with 24 hour time, then returns it to outputLocation
void formatTime(unsigned long seconds, char (&outputLocation)[MAX_TIME_CHARS + 1]) {

    unsigned long remainingSeconds;

    unsigned int hours = (unsigned int) ( seconds / 3600 );
    hours %= 24;

    unsigned int minutes;
    minutes = (unsigned int) ((seconds % 3600) / 60);

    remainingSeconds = (seconds % 60);

    sprintf(outputLocation, "%02u:%02u:%02lu", hours, minutes, remainingSeconds);

}