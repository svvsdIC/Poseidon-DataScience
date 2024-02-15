#ifndef __SERIAL_COMMAND_H__
#define __SERIAL_COMMAND_H__


#include "sensor.h"


#define MAX_COMMAND_PARAMETERS (3)

#define MAX_PARAMETER_LENGTH (24)

#define MAX_SERIAL_COMMAND_LENGTH (128)


//typedef int (*methodArray) ();


void parseSerialCommand(char command[MAX_SERIAL_COMMAND_LENGTH + 1]);



// serves as the base class for the different responses to serial commands
class SerialCommand_Base{

    public:
        virtual int executeCommand();
        SerialCommand_Base(char parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1]);
        virtual ~SerialCommand_Base();
    protected:
        char m_parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1];


};


class SerialCommand_Status : public SerialCommand_Base {
    public:
        SerialCommand_Status(char parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1]);
        int executeCommand();
};





#endif // #ifndef __SERIAL_COMMAND_H__