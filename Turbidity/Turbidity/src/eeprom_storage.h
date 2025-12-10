#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <Arduino.h>

// Load persisted `registerMap` from EEPROM (if present) into RAM
void loadRegisterMapFromEEPROM();

// Save current `registerMap` to EEPROM (uses EEPROM.put/update semantics)
void saveRegisterMapToEEPROM();

#endif // EEPROM_STORAGE_H
