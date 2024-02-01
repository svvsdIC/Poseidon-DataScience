#ifndef __EVENTLOGGER_H__
#define __EVENTLOGGER_H__

// The maximum characters in the name of a file
#define MAX_FILE_NAME_LENGTH (8 + 1 + 3 + 1)

class Event_Logger {
    public:
        
        char m_fileName[MAX_FILE_NAME_LENGTH];
        
        bool m_sendLogsOnSerial;

        Event_Logger(char fileName[MAX_FILE_NAME_LENGTH], bool sendLogsOnSerial);

        void LogError(char * errorText);

        void LogEvent(char * eventText);
};





#endif //#ifndef __EVENTLOGGER_H__
