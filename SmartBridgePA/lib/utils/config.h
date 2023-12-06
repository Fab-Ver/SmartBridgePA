#ifndef __CONFIG__
#define __CONFIG__

#define BLINK_PERIOD 2000
#define SMART_LIGHT_PERIOD 200
#define WATER_LEVEL_PERIOD 200

#define WL1 70
#define WL2 40
#define WL_MAX 2

typedef enum BlinkState {ON, OFF, BLINK_OFF} BlinkState;
typedef enum SmartLightState {LIGHT_ON, LIGHT_OFF, WAITING, SYS_OFF} SmartLightState;
typedef enum WaterLevelState {NORMAL, PRE_ALARM, ALARM, MANUAL} WaterLevelState;
typedef enum ManualState {MANUAL_ON, MANUAL_OFF} ManualState;

#endif