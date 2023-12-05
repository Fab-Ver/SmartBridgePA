#ifndef __COMMONS__
#define __COMMONS__

#include <Arduino.h>
#include "config.h"

extern WaterLevelState currWaterLevelState;
extern BlinkState currBlinkState;
extern SmartLightState currSmartLightState;
extern bool currDetectionState;
extern bool currDarkState;
extern SemaphoreHandle_t xMutex;

#endif