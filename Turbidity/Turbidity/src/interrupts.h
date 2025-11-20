#pragma once

void startI2C();
void updateTurbidity();
// void recordI2CAddress(void);
void receiveEvent(int numberOfBytesReceived);
void requestEvent();
void sendData();
void sendData(byte data);

//int responseCode; // Used to determine if the last command was successful or not