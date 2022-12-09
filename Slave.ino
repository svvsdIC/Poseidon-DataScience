// this is an i2c buss client (aka slave)
// it imitates an ATLAS SENSORl

#include "Wire.h"

void receiveEvent(int howMany);
void requestEvent();

#define SLAVE_ADDRESS (1)
#define SENSOR_DATA_SIZE 50

volatile byte rcvBuf[20]; // buffer for data from master
String sensorData = String("");

void setup() {

  Wire.begin(SLAVE_ADDRESS);     // join i2c bus with address
  //TWAMR = (98 | 99 | 97 | 100 | 102 ) << 1;
  Wire.onReceive(receiveEvent);   // register handler for data from master
  Wire.onRequest(requestEvent);   // register handler for master requests
  
  Serial.begin(9600);  // start serial for output
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

volatile uint8_t twi_TWAR = -8;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    for (int i = 0; Wire.available(); i++){ // get all bytes available
    rcvBuf[i] = tolower(Wire.read());     // force to lower  case
  }    
  rcvBuf[ howMany ] = '\0';    // make it a null terminated C string
    char myBuf[howMany + 1];
    strcpy(myBuf,rcvBuf); // can't print a volitile string
    Serial.print("Client  received:  ");Serial.println(myBuf);
}


// function that executes whenever data is requested by master.
// this function is registered as an event, see setup()
void requestEvent(){
  char myBuf[50];
  
  strcpy(myBuf,rcvBuf);
  if(strcmp(rcvBuf,"r")==0){  // read sensor data  
      byte response[SENSOR_DATA_SIZE];
      for (byte i = 0; i < SENSOR_DATA_SIZE; i++) {
        response[i] = (byte)sensorData.charAt(i);
      }
      Wire.write(1);    //  return code for 'successb
      for(int i=0; i<SENSOR_DATA_SIZE;i++){
        Wire.write(response[i]);
      }
      Serial.print("client ");Serial.print(twi_TWAR);Serial.print(" sent:  ");Serial.println(sensorData);
      Serial.println();
      return;      
  }else if( strcmp(rcvBuf,"find")==0){
    Wire.write(1); //return code for 'success'
    Wire.write('\0');
    Serial.println("start flashing");
  }else{
    Wire.write(2);  //return code for 'syntax error'
    Wire.write('\0');

  }
}
