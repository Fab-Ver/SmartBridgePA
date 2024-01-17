#ifndef __UTIL__
#define __UTIL__

#include "config.h"
#include "Arduino.h"

String convertWaterLevelState(WaterLevelState value){
    String names[4] = {"NORMAL", "PRE_ALARM", "ALARM", "MANUAL"};
    return names[value];
} 

String convertManualState(ManualState value){
    String names[2] = {"MANUAL_ON", "MANUAL_OFF"};
    return names[value];
} 

#endif