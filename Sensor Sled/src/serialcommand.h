#ifndef __SERIAL_COMMAND_H__
#define __SERIAL_COMMAND_H__

#define MAX_COMMAND_PARAMETERS (3)

#define MAX_PARAMETER_LENGTH (24)

#define MAX_SERIAL_COMMAND_LENGTH (127) // We add 1 to lengths and this aligns the data

// TODO: add description
class Serial_Command {
    public:
        Serial_Command();
        int parseAndRun(char command[MAX_SERIAL_COMMAND_LENGTH + 1]);

    protected:
        char m_parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1];
        
        int checkStatus();
};


#endif // #ifndef __SERIAL_COMMAND_H__