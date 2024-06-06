/*

File Description:

    Header file for utilites.cpp

*/

#ifndef __UTILITIES_H
#define __UTILITIES_H

// Maximum length of a formatted time string (hh:mm:ss)
#define MAX_TIME_CHARS (8)

// the header string "Electrical Conductivity,Total Dissolved Solids,Salinity,Specific Gravity,Oxygen Reduction,Dissolved Oxygen Density,Dissolved Oxygen Percent,Potential Hydrogen,Temperature\n"
// has 171 characters.  Update MAX_FILE_ROW_LENGTH with more parameters.

// The maximum characters in one row of a file stored on the μsd card
#define MAX_FILE_ROW_LENGTH (255)

// The maximum characters in the name of a file (8 character name, dot, 3 character extension, null terminator)
#define MAX_FILE_NAME_LENGTH (8 + 1 + 3 + 1)

//Takes an input in seconds and formats it into the form "hh:mm:ss"
// then returns it to outputLocation
void formatTime(unsigned long seconds, char (&outputLocation)[MAX_TIME_CHARS + 1]);

// Takes a csv-formatted string.  Creates a unique logfile on the μsd card and writes
// csv_header to the top of the file.  Returns 99 if writing to existing file (if
// sensorlog99.csv already exists) or 0 if writing to a new file.  Sends file name to fileNameLocation
int createDataFile(char csv_header[MAX_FILE_ROW_LENGTH+ 1], char (&fileNameLocation)[MAX_FILE_NAME_LENGTH]);

// takes a string of csv data and the name of a file and adds that data plus a line break
// to the named  file.  Returns 0 for success and 99 for faliure
int writeLineToFile(char data_line[MAX_FILE_ROW_LENGTH + 1], char fileName[MAX_FILE_NAME_LENGTH]);

#endif