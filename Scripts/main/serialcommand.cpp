#include "serialcommand.h"
#include <Arduino.h>


void parseSerialCommand(char command[MAX_SERIAL_COMMAND_LENGTH + 1]) {
    char commandCaps[MAX_SERIAL_COMMAND_LENGTH + 1];

    int i = 0;
    while(command[i]) {
        commandCaps[i] = (char) toupper(command[MAX_SERIAL_COMMAND_LENGTH]);
    }
    
}

SerialCommand_Base::~SerialCommand_Base() {};

SerialCommand_Base::SerialCommand_Base(char parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1]) {
    
    for (int i = 0; i < MAX_COMMAND_PARAMETERS; i++) {
        strncpy(m_parameters[i], parameters[i], MAX_PARAMETER_LENGTH);
    }
}


SerialCommand_Status::SerialCommand_Status(char parameters[MAX_COMMAND_PARAMETERS][MAX_PARAMETER_LENGTH + 1]) : SerialCommand_Base(parameters) {

}



int SerialCommand_Status::executeCommand() {
    return 999;
}