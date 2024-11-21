#include <Arduino.h>

constexpr int pin = 13;

void setup() {
    pinMode(pin, OUTPUT);  // Set pin 2 as output
}

void loop() {
    digitalWrite(pin, HIGH);  // Turn LED on
    delay(1000);            // Wait for a second
    digitalWrite(pin, LOW);   // Turn LED off
    delay(1000);            // Wait for a second
}