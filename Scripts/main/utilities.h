/*

File Description:

    Header file for utilites.cpp

*/

#ifndef __UTILITIES_H
#define __UTILITIES_H

// Maximum length of a formatted time string (hh:mm:ss)
#define MAX_TIME_CHARS (8)

// The maximum characters in one row of a CSV file
#define MAX_CSV_ROW_LENGTH (40)

//Takes an input in seconds and formats it into the form "hh:mm:ss"
// then returns it to outputLocation
void formatTime(unsigned long seconds, char (&outputLocation)[MAX_TIME_CHARS + 1]);

// Takes a sensorvalue, returns a row for a CSV file in the format "type,timeStamp,data\n"
void create_CSV_Row(SensorValue readingData, char (&outputStringLocation)[MAX_CSV_ROW_LENGTH + 1]);

#endif