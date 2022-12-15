// this is an i2c buss client (aka slave)
// it imitates an ATLAS SENSORl

#include "Wire.h"

// declare functions 
void receiveEvent(int howMany);
void requestEvent();
void ORP_handler(char* command);
void pH_handler(char* command);
void EC_handler(char* command);
void DO_handler(char* command);
void RTD_handler(char* command);

#define SLAVE_ADDRESS (1)     // using modified twi.c can use any address here

volatile char rcvBuf[20];     // buffer for data from master
String sensorData = String("");

enum expected_i2c_addresses {
  ORP =  98,
  pH  =  99,
  EC  = 100,
  DO  =  97,
  RTD = 102
};

void setup() {

  Wire.begin(SLAVE_ADDRESS);      // start the i2c bus
  Wire.onReceive(receiveEvent);   // register handler for data from master
  Wire.onRequest(requestEvent);   // register handler for master requests
  
  Serial.begin(9600);           // start serial for output
  Serial.println("client...");
  
  Serial.println();
  
  // set up led
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // blink the led to show where the chip is if 'find' command
  if(strcmp( rcvBuf, "find")==0){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    return;      
  }

  // calculate a sensor value
  sensorData = String(( sin(millis()/100 ) * 10) + 40.0); 
}        

extern volatile uint8_t twi_TWAR;  // in twi.c, this gets the i2c address
								   // used to issue a request for data

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    for (int i = 0; Wire.available(); i++){ // get all bytes available
    rcvBuf[i] = tolower(Wire.read());     // force to lower  case
  }    
  rcvBuf[ howMany ] = '\0';    // make it a null terminated C string
    char myBuf[howMany + 1];
    strcpy(myBuf, rcvBuf); // can't print a volitile string
    Serial.print("Client  received:  ");Serial.println(myBuf);
}


// function that executes whenever data is requested by master.
// this function is registered as an event, see setup()
void requestEvent(){
  char myBuf[50] = "";
  strncpy(myBuf, rcvBuf, 50);

  switch (twi_TWAR){
	case ORP:
		ORP_handler(myBuf);
    
		break;

	case pH:
		pH_handler(myBuf);
		break;

	case EC:
		EC_handler(myBuf);
		break;

	case DO:
		DO_handler(myBuf);
		break;

	case RTD:
		RTD_handler(myBuf);
		break;
 
  default:
    // do nothing
    Serial.print("unknown i2c address received: ");Serial.println(twi_TWAR);

  }
  
}

void sendResponse(char* response){

    int respLen = strlen(response);
    Wire.write(1);    //  return code for 'successb
    for(int i=0; i<=respLen;i++){
      Wire.write(response[i]);
    }
    Wire.write('0');
    Serial.print("client ");Serial.print(twi_TWAR);Serial.print(" sent:  ");Serial.println(response);
    Serial.println();
}

// specific Atlas Sensor handlers

void ORP_handler(char* command){
    sendResponse("ORP");
}

void pH_handler(char* command){
    sendResponse("pH");
}

void EC_handler(char* command){
    sendResponse("EC");
}

void DO_handler(char* command){
    sendResponse("DO");
}

void RTD_handler(char* command){
    sendResponse("RTD");
}
