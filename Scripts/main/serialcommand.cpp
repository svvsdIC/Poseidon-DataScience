#include <Arduino.h>
#include "serialcommand.h"
#include "sensor.h"


Serial_Command::Serial_Command() {}



int Serial_Command::parseAndRun(char command[MAX_SERIAL_COMMAND_LENGTH + 1]) {
    char commandCaps[MAX_SERIAL_COMMAND_LENGTH + 1];

    for(int i = 0; commandCaps[i] != '\0'; i++) {
        commandCaps[i] = (char) toupper(command[i]);
    }

    // separate by commas, with the first value as the command name 
    // and the following as parameters passed to that command

    char commandName[MAX_PARAMETER_LENGTH + 1];

    char parameter[MAX_PARAMETER_LENGTH + 1];

    int p = 0;

    for (int i = 0; commandCaps[i] != '\0'; i++) { 

        char recievedByte = commandCaps[i];

        if(recievedByte != ',') { 
        
            // add byte to data string
            parameter[i] = recievedByte;
        
        } else { // is a comma
            
            if(p >= MAX_COMMAND_PARAMETERS) {
                // too may commas
                // TODO: add error response based on return
                return 999;
            } else {
                strncpy(m_parameters[p], parameter, MAX_PARAMETER_LENGTH);
            }

            for (int k = 0; k < MAX_PARAMETER_LENGTH + 1; k++) {
                parameter[k] = '\0';
            }

            // we want to start at character 0 next time we get here,
            // so i is set to -1 since i increments before we get here.
            i = -1;
            p++;

        } 


        if(recievedByte == '\0') {
            // end of string
            strncpy(m_parameters[p - 1], parameter, MAX_PARAMETER_LENGTH);
            strcpy(parameter, "");
            break;
        }
    }


    if(m_parameters[0] == "STATUS") {
        Serial.println(commandName);
        Serial.println(m_parameters[0]);
    // execute corresponding method
    
    } else if (commandName == "") {
    
    } else {
        // command not valid
        return 998;
    }


    return 1;
    
}


// **************************** Commands below this point *************************

int Serial_Command::checkStatus() {


    return 0;

}