// this is an i2c bus client (aka slave)
// it imitates a collection of ATLAS SENSORS
//
// It uses modified Wire.c twi.c modules to allow the reception of
// multiple i2c address

#include "Wire.h"

extern volatile uint8_t twi_TWAR;  // in twi.c, this holds the i2c address
                                   // used to issue a request for data


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
long startMillis = millis();  // used to time captures of calcCounter
long calcCounter = 0;         // counter used to help create pseudo sensor values
int  counterDirection = 1;    // used to let values go up and down

enum expected_i2c_addresses {
  ORP =  98,
  pH  =  99,
  EC  = 100,
  DO  =  97,
  RTD = 102   
};

// trim leading characters from a char string
char* trim(char *c) {
    while(*c && isspace(*c)) c++;
    return c;
}

void setup() {

  Wire.begin(SLAVE_ADDRESS);      // start the i2c bus
  Wire.onReceive(receiveEvent);   // register handler for data from master
  Wire.onRequest(requestEvent);   // register handler for master requests
  
  Serial.begin(9600);             // start serial for output
  Serial.println("client...\n");
  
  // set up led
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}



void loop() {
  // blink the led to show where the chip is if 'find' command
  //this command is foudn on all ATLAS sensors
  if(strcmp( rcvBuf, "find")==0){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    return;      
   }

  // manage counter used for calculating pseudo sensor values
  // once per second, varies from 0..100..0..100...
  if( millis()-startMillis > 1000){
    startMillis = millis();
    calcCounter += counterDirection;
    if(calcCounter <= 0 or calcCounter >= 100){
      Serial.println("flip...");
      counterDirection *= -1;
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    // get all data from controller in lower case
    for (int i = 0; Wire.available(); i++){ 
      rcvBuf[i] = tolower(Wire.read());
    }    
    
    rcvBuf[ howMany ] = '\0';    // make it a null terminated C string
    char myBuf[howMany + 1];
    strcpy(myBuf, rcvBuf);       // can't print a volitile string
    Serial.print("Client  received:  ");Serial.println(myBuf);
}


// function that executes whenever data is requested by controller
// this function is registered as an event, see setup()
void requestEvent(){
  // convert volatile rcfBuf to local string
  char myBuf[50] = "";
  strncpy(myBuf, rcvBuf, 50);

  // using i2c address sent by controller in request
  // call appropriate handler routines
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

// routine for sending a response to the controller 
void sendResponse(char* response){
    int respLen = strlen(response);

    Wire.write(1);    //  return code for 'success"
    
    for(int i=0; i<=respLen;i++){
      Wire.write(response[i]);
    }
    Serial.print("client ");Serial.print(twi_TWAR);Serial.print(" sent:  ");Serial.println(response);
    Serial.println();
}


/////////////////////////////////////////////////////////////////
// specific simulated Atlas Sensor request handlers
/////////////////////////////////////////////////////////////////
void ORP_handler(char* command){
    // calculate a simulated oxidation/reduction potential between -1019.9 - +1019.9 mv
    // that varies slowly
    float tempFloat = map(calcCounter, 0, 100, -10199, 10199) / 10.0;
    char tempValue[20];
    dtostrf(tempFloat,7,1,tempValue);  // eg:  8.123

    // return it to the controller over the i2c bus
    sendResponse(trim(tempValue));
  }

void pH_handler(char* command){
    // calculate a simulated acidity between .001 - 14.000
    // that varies slowly
    float tempFloat = map(calcCounter, 0, 100, 1, 14000) / 1000.0;
    char tempValue[20];
    dtostrf(tempFloat,6,3,tempValue);  // eg:  8.123

    // return it to the controller over the i2c bus
    sendResponse(trim(tempValue));
  }

void EC_handler(char* command){
  if(command[0]=='r'){
    // calculate simulated values of
    //   conductivity (ec): 1000 - 3000 uS/cm
    //   total dissolved solids (tds): 1000 - 20,000 ppm
    //   salinity:  0 - 42 psu(s) (practical salinity units, or parts per thousand)
    //   specific gravity (sg):  1 - 1.3 (no units, is ratio of two densities, test-liquid/4-deg-C-water)
    float ecFloat = map(calcCounter, 0, 100, 1000, 3000);
    char  ecValue[20];
    dtostrf(ecFloat,4,0,ecValue);
    float tdsFloat = map(calcCounter, 0, 100, 1000, 20000);
    char  tdsValue[20]; 
    dtostrf(tdsFloat, 5, 0, tdsValue);
    float sFloat = map(calcCounter, 0, 100, 0, 4200) / 100.0;
    char  sValue[20];
    dtostrf(sFloat, 2, 0, sValue);
    float sgFloat = map(calcCounter, 0, 100, 0, 13) / 10.0;
    char sgValue[20];
    dtostrf(sgFloat, 3, 1, sgValue);

    char response[20];
    snprintf( response, 
              20, 
              "%s,%s,%s,%s",
              trim(ecValue),
              trim(tdsValue),
              trim(sValue),
              trim(sgValue)            
    );

    // return it to the controller over the i2c bus
    sendResponse(response);
  }
}

void DO_handler(char* command){
  if(command[0]=='r'){
    // calculate simulated  disolved oxygen 0.01 - 100.00 mg/L
    // calculate simulated % saturation .01 0 400.00
    // that varies slowly  6
    float mg_per_liter_float = map(calcCounter, 0, 100, 1, 1500) /100.0;
    char mg_per_liter_value[20];
    dtostrf(mg_per_liter_float,4,2,mg_per_liter_value);
    float percent_float = map(calcCounter, 0, 100, 1, 10000) / 100;
    char percent_value[20];
    dtostrf(percent_float,7,2,percent_value);

    char response[20];
    snprintf( response,
              20, 
              "%s,%s", 
              trim(mg_per_liter_value), 
              trim(percent_value)
    );

    // return it to the controller over the i2c bus
    sendResponse(response);
  }
}

void RTD_handler(char* command){
  if(command[0]=='r'){
    // calculate a simulated celcius temperature between -10 and 35
    // that varies slowly
    float tempFloat = map(calcCounter, 0, 100, -1000, 3500) /100.0;
    char tempValue[20];
    dtostrf(tempFloat,8,3,tempValue);  // eg: -126.000

    // return it to the controller over the i2c bus
    sendResponse(trim(tempValue));
  }
}
