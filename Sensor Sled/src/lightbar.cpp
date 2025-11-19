#include "lightbar.h"

LightBar::LightBar(int LEDPin, int LaserPin)
  : m_LEDPin(LEDPin), m_laserPin(LaserPin), m_currentLEDMode(LIGHTBAR_MODE::OFF),
    m_currentLaserMode(LIGHTBAR_MODE::OFF) {}

LightBar::~LightBar()
{
    setMode(LIGHTBAR_MODE::OFF);
    setLaserMode(LIGHTBAR_MODE::OFF);
}

void LightBar::setMode(LIGHTBAR_MODE mode) {
    m_currentLEDMode = mode;

    int pwmValue = LIGHTBAR_MODE_MAP[mode];

    analogWrite(m_LEDPin, pwmValue);
}

void LightBar::toggleLaser() {
    if (m_currentLaserMode == LIGHTBAR_MODE::OFF) {
        setLaserMode(LIGHTBAR_MODE::MAX);
    } else {
        setLaserMode(LIGHTBAR_MODE::OFF);
    }
}

void LightBar::setLaserMode(LIGHTBAR_MODE mode) {
    m_currentLaserMode = mode;

    int pwmValue = LIGHTBAR_MODE_MAP[mode]; // Laser is either off or full power

    analogWrite(m_laserPin, pwmValue);
}