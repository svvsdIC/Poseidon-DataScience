#ifndef _LIGHTBAR_H__
#define _LIGHTBAR_H__
#include <Arduino.h>
#include <unordered_map>
/*
This file should provide an api to a light bar (pin tbd)
The light bar should have 5 power levels, (off, low, med, high, max)

*/

// Abbreviations because LOW and HIGH are already taken by arduino
enum LIGHTBAR_MODE {
    OFF = 0,
    L,       // Low
    M,       // Medium
    H,       // High
    MAX
};

int LIGHTBAR_MODE_MAP[5] = {
    0,
    64,
    128,
    192,
    255,
};

class LightBar {
    public:
        LightBar(int LEDPin, int LaserPin);
        ~LightBar();
        void setMode(LIGHTBAR_MODE mode);
        void toggleLaser();
        void setLaserMode(LIGHTBAR_MODE mode);

        inline const bool getLaserState() const [[nodiscard]] {
            return (m_currentLaserMode != LIGHTBAR_MODE::OFF);
        }
    private:
        int m_LEDPin;
        int m_laserPin;
        LIGHTBAR_MODE m_currentLEDMode;
        LIGHTBAR_MODE m_currentLaserMode;
}; 

#endif //_LIGHTBAR_H__