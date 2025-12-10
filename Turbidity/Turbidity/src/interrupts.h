#pragma once

void startI2C();
void updateTurbidity();

void receiveEvent(int numberOfBytesReceived);
void requestEvent();
void sendData();
void sendData(byte data);