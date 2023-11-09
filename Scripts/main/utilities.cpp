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

// takes a string of csv data and the name of a file and adds that data plus a line break
// to the named  file.
int logData(char csv_data[MAX_CSV_ROW_LENGTH], char fileName[MAX_FILE_NAME_LENGTH]) {

    File logFile;
    
    logFile = SD.open(fileName, FILE_WRITE);
	if( !logFile ) {
		Serial.print("Could not open "); 
		Serial.println(fileName);
		return 1;
	}

    logFile.println(csv_data);

    logFile.close();

}


/*
// Takes a sensorvalue, returns a row for a CSV file in the format "type,timeStamp,data\n"
void create_CSV_Row(SensorValue readingData, char (&outputStringLocation)[MAX_CSV_ROW_LENGTH + 1]) {
    char finalCSVRow[MAX_CSV_ROW_LENGTH + 1];
    sprintf(finalCSVRow, "%0i,%0lu,%0lf\n", (int) readingData.type, readingData.timeStamp, readingData.value);
    strncpy(outputStringLocation, finalCSVRow, MAX_CSV_ROW_LENGTH);
}
*/