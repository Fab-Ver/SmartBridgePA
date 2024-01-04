#ifndef __UTIL__
#define __UTIL__

#include "config.h"
#include "Arduino.h"

String convertSmartLightState(SmartLightState value){
    String names[4] = {"LIGHT_ON", "LIGHT_OFF", "WAITING", "SYS_OFF"};
    return names[value];
}

String convertWaterLevelState(WaterLevelState value){
    String names[4] = {"NORMAL", "PRE_ALARM", "ALARM", "MANUAL"};
    return names[value];
} 

String convertManualState(ManualState value){
    String names[2] = {"MANUAL_ON", "MANUAL_OFF"};
    return names[value];
} 

#endif