#include <EEPROM.h>
#include "eeprom_storage.h"
#include "constants.h"
#include <string.h>

// Magic value to detect a valid EEPROM region
static const uint16_t EEPROM_MAGIC = 0xA5A5;
static const int EEPROM_BASE_ADDR = 0; // start address in EEPROM
static const int EEPROM_MAGIC_SIZE = sizeof(EEPROM_MAGIC);

void loadRegisterMapFromEEPROM()
{
    uint16_t magic = 0;
    EEPROM.get(EEPROM_BASE_ADDR, magic);

    int dataAddr = EEPROM_BASE_ADDR + EEPROM_MAGIC_SIZE;

    if (magic == EEPROM_MAGIC)
    {
        // Magic matches, read saved registerMap after the magic
        // Read into a temporary (non-volatile) copy then assign into the
        // volatile registerMap to avoid casting away volatile.
        memoryMap tmp;
        EEPROM.get(dataAddr, tmp);
        // Copy into the volatile registerMap using memcpy to avoid requiring
        // a volatile qualified copy assignment/copy constructor.
        memcpy((void *)&registerMap, (const void *)&tmp, sizeof(memoryMap));
    }
    else
    {
        // First run or invalid data, write current defaults to EEPROM
        EEPROM.put(EEPROM_BASE_ADDR, EEPROM_MAGIC);
        // Copy from volatile registerMap into a non-volatile temporary for EEPROM
        memoryMap tmp;
        memcpy((void *)&tmp, (const void *)&registerMap, sizeof(memoryMap));
        EEPROM.put(dataAddr, tmp);
    }
}

void saveRegisterMapToEEPROM()
{
    // Save current registerMap to EEPROM after the magic header
    int dataAddr = EEPROM_BASE_ADDR + EEPROM_MAGIC_SIZE;
    // EEPROM.put internally uses EEPROM.update semantics and will avoid
    // rewriting bytes that are already identical (reduces wear).
    memoryMap tmp;
    memcpy((void *)&tmp, (const void *)&registerMap, sizeof(memoryMap));
    EEPROM.put(dataAddr, tmp);
}
