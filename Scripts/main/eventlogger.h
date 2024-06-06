#ifndef __EVENTLOGGER_H__
#define __EVENTLOGGER_H__

// The maximum characters in the name of a file
#define MAX_FILE_NAME_LENGTH (8 + 1 + 3 + 1)

// Class which defines the creation and useage of an error and event log stored on an μSD card
class Event_Logger {
    public:
        
        // String containing the name of the log file
        char m_fileName[MAX_FILE_NAME_LENGTH];
        
        // Whether to send a duplicate of each event or error over the serial line
        bool m_sendLogsOnSerial;

        // Constructor for the Event_Logger class.  Takes the name of the log file, and whether to send logs over the serial line.
        Event_Logger(const char fileName[MAX_FILE_NAME_LENGTH], bool sendLogsOnSerial);

        // Method to add an error to the event log.  Takes a pointer to the error text.
        void LogError(const char * errorText);

        // Method to add an event to the event log.  Takes a pointer to the event text.
        void LogEvent(const char * eventText);
};





#endif //#ifndef __EVENTLOGGER_H__
