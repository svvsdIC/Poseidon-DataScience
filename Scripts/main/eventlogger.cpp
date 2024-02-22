#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "eventlogger.h"
#include "utilities.h"

Event_Logger::Event_Logger(const char fileName[MAX_FILE_NAME_LENGTH], bool sendLogsOnSerial) {
    strncpy(this->m_fileName, fileName, MAX_FILE_NAME_LENGTH);
    m_sendLogsOnSerial = sendLogsOnSerial;
}


void Event_Logger::LogError(const char * errorText) {
    File eventLogFile;
    
    eventLogFile = SD.open(m_fileName, FILE_WRITE);
	if( !eventLogFile ) {
		Serial.print("Could not open "); 
		Serial.println(m_fileName);
	}

    char timeStamp[MAX_TIME_CHARS + 1];

    formatTime((unsigned long) (( (float) millis() ) / 1000), timeStamp);

    
    char errorLine[MAX_FILE_ROW_LENGTH + 1];

    snprintf(errorLine, MAX_FILE_ROW_LENGTH, "%s ERROR: %s", timeStamp, errorText);

    eventLogFile.println(errorLine);

    if(m_sendLogsOnSerial) {
        Serial.println(errorLine);
    }

    eventLogFile.close();
}


void Event_Logger::LogEvent(const char * eventText) {
    File eventLogFile;
    
    eventLogFile = SD.open(m_fileName, FILE_WRITE);
	if( !eventLogFile ) {
		Serial.print("Could not open "); 
		Serial.println(m_fileName);
	}

    char timeStamp[MAX_TIME_CHARS + 1];

    formatTime((unsigned long) (( (float) millis() ) / 1000), timeStamp);

    
    char eventLine[MAX_FILE_ROW_LENGTH + 1];

    snprintf(eventLine, MAX_FILE_ROW_LENGTH, "%s EVENT: %s", timeStamp, eventText);

    eventLogFile.println(eventLine);

    if(m_sendLogsOnSerial) {
        Serial.println(eventLine);
    }

    eventLogFile.close();
}