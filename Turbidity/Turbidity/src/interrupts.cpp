#include <EEPROM.h>
#include <Wire.h>
#include "constants.h"
#include "interrupts.h"
#include "eeprom_storage.h"
#include <string.h>

// The chip will need to send a response code. This will be a variable
// and we set it whenever something happens. We then can return in on req
#define SUCCESS 1
int responseCode = SUCCESS;

enum class CurrentRead
{
    NONE = 0,
    TURBIDITY,
    CALIBRATION,
    SAMPLE_PARAMS,
};

CurrentRead currentRead = CurrentRead::NONE;

// Begin listening on I2C bus as I2C slave using the global variable setting_i2c_address
void startI2C()
{
    Wire.end(); // Before we can change addresses we need to stop

    Wire.begin(registerMap.i2cAddress); // Start I2C and answer calls using address from EEPROM

    // The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

// Gets the photoresistor values in the MSB and LSB variables
void updateTurbidity()
{
    // TODO: Variable average sample delay over I2C
    // TODO: Variable amount of samples to average over I2C
    // Bring up ADC
    // Analog to digital converter.
    ADCSRA |= (1 << ADEN);

    digitalWrite(Light_Pin, HIGH); // Turn on light to maximum brightness
    delay(10);                   // Wait for light to stabilize

    int photoTotal = 0;
    for (int i = 0; i < registerMap.numSamples; i++)
    {
        photoTotal += analogRead(Photoresistor_Pin); // Read the ADC value from the photoresistor
        delay(registerMap.msReadDelay); // Wait in between readings
    }

    digitalWrite(Light_Pin, LOW); // Turn off light

    uint16_t photo = photoTotal / registerMap.numSamples; // Average the 8 readings
    // Idk if we need to AND with 0xFF
    registerMap.PhotoMSB = (photo >> 8) & 0xFF; // Get the MSB of the ADC value
    registerMap.PhotoLSB = photo & 0xFF;        // Get the LSB of the ADC value

    // Shut off ADC
    ADCSRA &= ~(1 << ADEN);
}

/*This should handle the strings (commands) that are sent
  Right now it doesn't do anything. I just acks the command*/
// When Qwiic Joystick receives data bytes from Master, this function is called as an interrupt
//(Serves rewritable I2C address)
void receiveEvent(int numberOfBytesReceived)
{
    //registerNumber = Wire.read(); // Get the memory map offset from the user
    char command[128]; // Buffer for command
    int index = 0;
    while (Wire.available())
    {
        byte character = Wire.read();
        // If capital letter, make lowercase
        if (character >= 'A' && character <= 'Z')
        {
            character += ('a' - 'A');
        }
        command[index] = character; // Read each byte into the command buffer
        index++;
        if (index >= 127) // Prevent buffer overflow
        {
            break;
        }
    }
    // Null-terminate the command string
    if (index < 128)
    {
        command[index] = '\0';
    }
    if (strcmp(command, "r") == 0) // Read sensor
    {
        // Update sensor data when a command 'R' is received
        updateTurbidity();

        currentRead = CurrentRead::TURBIDITY;
    }
    if (strcmp(command, "import") == 0) // Imports calibration data
    {
        // Will be implemented when more is known about turbidity calibration
    }
    if (strcmp(command, "export") == 0) // Exports calibration data
    {
        currentRead = CurrentRead::CALIBRATION;
    }
    if (strncmp(command, "setsamp", 7) == 0) // Imports sample parameters, format of (num samples, ms between samples)
    {
        // Read command after "setsamp" until whitespace for number of samples
        char numSamplesStr[8];
        char msBetweenStr[8];

        int i = 7; // Start after "setsamp"
        int j = 0;

        // Read number of samples
        while (command[i] != ' ' && command[i] != '\0' && j < 7)
        {
            numSamplesStr[j] = command[i];
            i++;
            j++;
        }
        numSamplesStr[j] = '\0'; // Null terminate
        i++;                     // Skip whitespace
        j = 0;

        // Read ms between samples
        while (command[i] != ' ' && command[i] != '\0' && j < 7)
        {
            msBetweenStr[j] = command[i];
            i++;
            j++;
        }
        msBetweenStr[j] = '\0'; // Null terminate

        // Convert to integers and save
        registerMap.numSamples = atoi(numSamplesStr);
        registerMap.msReadDelay = atoi(msBetweenStr);
        // Persist these changes to EEPROM
        saveRegisterMapToEEPROM();
    }
    if (strcmp(command, "getsamp") == 0)
    {
        currentRead = CurrentRead::SAMPLE_PARAMS;
    }
}

void sendTurbidityData();
void sendData();
// Respond to GET commands
// The interrupt will respond with bytes starting from the last byte the user sent to us
// While we are sending bytes we may have to do some calculations
void requestEvent()
{
    // Should send the response code first, then the actual data. Doesn't work
    sendData(responseCode); // Send the response code when a request is made
    sendData();
}

// Really bad overload structure
void sendTurbidityData()
{
  // TODO: Delete this when done testing
  updateTurbidity();
  
  // Doing this instead of register variable values because this should be
  // the only value being read from the sensor
  sendData(registerMap.PhotoMSB);
  sendData(registerMap.PhotoLSB);
  // Expected by the master
  sendData('\0'); // Send a null terminator to indicate the end of the data
}
  
  void sendData()
  {
    switch (currentRead)
    {
    case CurrentRead::TURBIDITY:
      sendTurbidityData();
      break;
    case CurrentRead::CALIBRATION:
      break;
    case CurrentRead::SAMPLE_PARAMS:
      sendData((byte)registerMap.numSamples);
      sendData((byte)registerMap.msReadDelay);
      sendData('\0'); // End of data
      break;
    }
    
    currentRead = CurrentRead::NONE; // Reset current read
  }

  void sendData(byte data)
  {
      Wire.write(data); // Send the data to the master
  }