#include <SPI.h>
#include <SD.h>
#include <Arduino.h>

// Set the pins used
#define cardSelect 4

File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}


void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);

  delay(3000);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }

  Serial.println("Card init succeeded");

  char filename[15];
  strcpy(filename, "/log00.csv");
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = '0' + i/10;
    filename[5] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

	logfile = SD.open(filename, FILE_WRITE);
	if( ! logfile ) {
		Serial.print("Couldnt create "); 
		Serial.println(filename);
		error(3);
	}
	Serial.print("Writing to "); 
	Serial.println(filename);

	pinMode(13, OUTPUT);
	pinMode(8, OUTPUT);
	Serial.println("Ready!");

	logfile.println("Reading,Value");

  	for(int i = 0; i <= 100; i++) {

		digitalWrite(8, HIGH);
		logfile.print(i);
		logfile.print(",");
		logfile.println(analogRead(0));

		Serial.print("A0 = "); Serial.println(analogRead(0));
		digitalWrite(8, LOW);
		
		delay(100);
	}

	logfile.close();
	Serial.println("Log complete");

}

void loop() {}