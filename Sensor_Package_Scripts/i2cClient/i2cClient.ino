// this is an i2c bus client (aka slave)
// it imitates a collection of ATLAS SENSORS
//
// It uses modified Wire.c twi.c modules to allow the reception of
// multiple i2c address

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
long startMillis = millis();  // used to time captures of calcCounter
long calcCounter = 0;         // counter used to help create pseudo sensor values
int  counterDirection = 1;    // used to let values go up and down

enum expected_i2c_addresses {
  ORP =  98,
  pH  =  99,
  EC  = 100,
  DO  =  97,
  RTD = 102   //TODO:  should be 102
};

void setup() {

  Wire.begin(SLAVE_ADDRESS);      // start the i2c bus
  Wire.onReceive(receiveEvent);   // register handler for data from master
  Wire.onRequest(requestEvent);   // register handler for master requests
  
  Serial.begin(9600);             // start serial for output
  Serial.println("client....");
  
  Serial.println();
  
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

  // increment counter used for calculating pseudo sensor values
  // once every second on a 180 second period
  if( millis()-startMillis > 1000){
    startMillis = millis();
    calcCounter += counterDirection;
    if(calcCounter < 0 or calcCounter > 100){
      Serial.println("flip...");
      counterDirection *= -1;
    }
  }
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
  if(command[0]=='r'){
    // request to read data

    // calculate a simulated values of
    //   conductivity (ec): 1000 - 3000 uS/cm
    //   total dissolved solids (tds): 1000 - 20,000 ppm
    //   salinity:  0 - 42 psu(s) (practical salinity units, or parts per thousand)
    //   specific gravity (sg):  1 - 1.3 (no units, is ratio of two densities, test-liquid/4-deg-C-water)
    float ecFloat =(  abs(sin(millis()/2000)) * 2000) + 1000;   // elec conductivity 
    char  ecValue[20];
    dtostrf(ecFloat,4,0,ecValue);
    float tdsFloat =( abs(sin(millis()/2000)) * 20000) + 1000;
    char  tdsValue[20]; 
    dtostrf(tdsFloat, 5, 0, tdsValue);
    float sFloat   =( abs(sin(millis()/2000)) *    42) + 0;
    char  sValue[20];
    dtostrf(sFloat, 2, 0, sValue);
    float sgFloat  =( abs(sin(millis()/2000)) *   1.3) + 1;
    char sgValue[20];
    dtostrf(sgFloat, 3, 1, sgValue);

    char response[20];
    snprintf( response, 
              20, 
              "%s,%s,%s,%s",
              ecValue,
              tdsValue,
              sValue,
              sgValue            
    );

    // return it to the controller over the i2c bus
    sendResponse(response);
  }
}

void DO_handler(char* command){
  if(command[0]=='r'){
    // calculate simulated  disolved oxygen 0 - 7 mg/L
    // calculate simulated % saturation 50% -  99%
    // that varies slowly  6
    float mg_per_liter_float = ( abs(sin(millis()/2000)) *  7) +  0;  //  0 - 7
    char mg_per_liter_value[20];
    dtostrf(mg_per_liter_float,4,2,mg_per_liter_value);
    float percent_float = ( abs(sin(millis()/2000)) * 49) + 50;       // 50 - 99
    char percent_value[20];
    dtostrf(percent_float,3,2,percent_value);

    char response[20];
    snprintf( response,
              20, 
              "%s,%s", 
              mg_per_liter_value, 
              percent_value
    );

    // return it to the controller over the i2c bus
    sendResponse(response);
  }
}

void RTD_handler(char* command){
  if(command[0]=='r'){
    // request to read data

    // calculate a simulated celcius temperature between -10 and 35
    // that varies slowly
    float tempFloat = map(calcCounter, 0, 100, -1000, 3500) /100.0;
    char tempValue[20];
    dtostrf(tempFloat,8,3,tempValue);  // eg: -126.000

    // return it to the controller over the i2c bus
    sendResponse(tempValue);
  }
}
