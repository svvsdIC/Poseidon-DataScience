#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File fd;
const uint8_t BUFFER_SIZE = 20;
char fileName[] = "demoFile.txt"; // SD library only supports up to 8.3 names
char buff[BUFFER_SIZE+2] = "";  // Added two to allow a 2 char peek for EOF state
uint8_t index = 0;

// 53 for mega, 10 for artemis
const uint8_t chipSelect = 10;
const uint8_t cardDetect = 9;

enum states: uint8_t { NORMAL, E, EO };
uint8_t state = NORMAL;

bool alreadyBegan = false;  // SD.begin() misbehaves if not first call



////////////////////////////////////////////////////////////////////////////////
// Standard Arduino setup function
////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(57600);
  while (!Serial);  // Wait for serial port to connect (ATmega32U4 type PCBAs)

  // Note: To satisfy the AVR SPI gods the SD library takes care of setting
  // SS_PIN as an output. We don't need to.
  pinMode(cardDetect, INPUT);

  initializeCard();
}



////////////////////////////////////////////////////////////////////////////////
// Arduino calls this function over and over again when running
////////////////////////////////////////////////////////////////////////////////
void loop()
{
  // Make sure the card is still present
  if (!digitalRead(cardDetect))
  {
    initializeCard();
  }

  if (Serial.available() > 0)
  {
    readByte();

    if (index == BUFFER_SIZE)
    {
      flushBuffer();  // Write full buffer to µSD card
    }
  }
}



////////////////////////////////////////////////////////////////////////////////
// Do everything from detecting card through opening the demo file
////////////////////////////////////////////////////////////////////////////////
void initializeCard(void)
{
  Serial.print(F("Initializing SD card..."));

  // Is there even a card?
  if (!digitalRead(cardDetect))
  {
    Serial.println(F("No card detected. Waiting for card."));
    while (!digitalRead(cardDetect));
    delay(250); // 'Debounce insertion'
  }

  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.

  bool chipSelectSuccess = SD.begin(chipSelect);
  if (!chipSelectSuccess && !alreadyBegan)  // begin uses half-speed...
  {
    Serial.println(F("Initialization failed!"));
    initializeCard(); // Possible infinite retry loop is as valid as anything
  }
  else
  {
    alreadyBegan = true;
  }
  Serial.println(F("Initialization done."));

  Serial.print(fileName);
    if (SD.exists(fileName))
    {
        Serial.println(F(" exists."));
    }
    else
    {
        Serial.println(F(" doesn't exist. Creating."));
    }

  Serial.print("Opening file: ");
  Serial.println(fileName);

  Serial.println(F("Enter text to be written to file. 'EOF' will terminate writing."));
}


////////////////////////////////////////////////////////////////////////////////
// This function is called after the EOF command is received. It writes the
// remaining unwritten data to the µSD card, and prints out the full contents
// of the log file.
////////////////////////////////////////////////////////////////////////////////
void eof(void)
{
  index -= 3; // Remove EOF from the end
  flushBuffer();

  // Re-open the file for reading:
    fd = SD.open(fileName);
    if (fd)
    {
        Serial.println("");
        Serial.print(fileName);
        Serial.println(":");

        while (fd.available())
        {
        Serial.write(fd.read());
        }
    }
    else
    {
        Serial.print("Error opening ");
        Serial.println(fileName);
    }
    fd.close();
    }


////////////////////////////////////////////////////////////////////////////////
// Write the buffer to the log file. If we are possibly in the EOF state, verify
// that to make sure the command isn't written to the file.
////////////////////////////////////////////////////////////////////////////////
void flushBuffer(void)
{
    fd = SD.open(fileName, FILE_WRITE);
    if (fd) {
        switch (state)  // If a flush occurs in the 'E' or the 'EO' state, read more to detect EOF
        {
        case NORMAL:
            break;
        case E:
            readByte();
            readByte();
            break;
        case EO:
            readByte();
            break;
        }
        fd.write(buff, index);
        fd.flush();
        index = 0;
        fd.close();
    }
}



////////////////////////////////////////////////////////////////////////////////
// Reads a byte from the serial connection. This also maintains the state to
// capture the EOF command.
////////////////////////////////////////////////////////////////////////////////
void readByte(void)
{
  byte byteRead = Serial.read();
  Serial.write(byteRead); // Echo
  buff[index++] = byteRead;

  // Must be 'EOF' to not get confused with words such as 'takeoff' or 'writeoff'
    if (byteRead == 'E' && state == NORMAL)
    {
        state = E;
    }
    else if (byteRead == 'O' && state == E)
    {
        state = EO;
    }
    else if (byteRead == 'F' && state == EO)
    {
        eof();
        state = NORMAL;
    }
}