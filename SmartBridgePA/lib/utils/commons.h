#ifndef __COMMONS__
#define __COMMONS__

#include <Arduino.h>
#include "config.h"

/*Water Level Task shared variables */
extern WaterLevelState currWaterLevelState;
extern ManualState currManualState;
extern float currWaterLevel;
extern int currValveAngle;
extern bool currGreenLedON;
extern bool currRedLedON;
/*---------------------------------*/

/*Blink Task shared variables      */
extern BlinkState currBlinkState;
/*--------------------------------*/

/*Smart Light Task shared variables*/
extern SmartLightState currSmartLightState;
extern SmartLightLedState currSmartLightLedState;
extern bool currDetectionState;
extern bool currDarkState;
/*--------------------------------*/

extern SemaphoreHandle_t xMutex;

#endif