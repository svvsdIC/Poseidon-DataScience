/*

File Description:

    Header file for utilites.cpp

*/

#ifndef __UTILITIES_H
#define __UTILITIES_H

// Maximum length of a formatted time string (hh:mm:ss)
#define MAX_TIME_CHARS (8)

//Takes an input in seconds and formats it into the form "hh:mm:ss"
// then returns it to outputLocation
void formatTime(unsigned long seconds, char (&outputLocation)[MAX_TIME_CHARS + 1]);


#endif