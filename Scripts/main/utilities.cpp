/*

    File Description:

        Contains miscellaneous functions needed in main.ino that do not pertain
        directly to Atlas sensors.

*/

#include <Arduino.h>
#include "utilities.h"
#include "sensor.h"
#include <SPI.h>
#include <SD.h>

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

// takes a string of and the name of a file and adds that data plus a line break
// to the named  file.  Returns 0 for success and 99 for faliure
int writeLineToFile(char text_line[MAX_FILE_ROW_LENGTH + 1], char fileName[MAX_FILE_NAME_LENGTH]) {

    File dataFile;
    
    dataFile = SD.open(fileName, FILE_WRITE);
	if( !dataFile ) {
		Serial.print("Could not open "); 
		Serial.println(fileName);
		return 99;
	}

    dataFile.println(text_line);

    dataFile.close();

    return 0;

}
// Takes a csv-formatted string.  Creates a unique logfile on the μsd card and writes
// csv_header to the top of the file.  Returns 99 if writing to existing file (if
// sensorlog99.csv already exists) or 0 if writing to a new file.  Sends file name to fileNameLocation
int createDataFile(char csv_header[MAX_FILE_ROW_LENGTH+ 1], char (&fileNameLocation)[MAX_FILE_NAME_LENGTH]) {
    char fileName[MAX_FILE_NAME_LENGTH];
    strncpy(fileName, "/data00.csv", MAX_FILE_NAME_LENGTH);

    for (uint8_t i = 0; i < 100; i++) {
        fileName[5] = '0' + i/10;
        fileName[6] = '0' + i%10;
        // create if does not exist, do not open existing, write, sync after write
        if (! SD.exists(fileName)) {
            break;
        }
    }

    strncpy(fileNameLocation, fileName, MAX_FILE_NAME_LENGTH);

    writeLineToFile(csv_header, fileName);
    if (SD.exists(fileName)) {
        return 99;
    } else {
        return 0;
    }
}