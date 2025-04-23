#pragma once

void startI2C();
void updateJoystick();
void recordI2CAddress(void);
void receiveEvent(int numberOfBytesReceived);
void requestEvent();