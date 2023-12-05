#ifndef __CONFIG__
#define __CONFIG__

#define BLINK_PERIOD 2000

typedef enum BlinkState {ON, OFF, BLINK_OFF} BlinkState;
typedef enum SmartLightState {LIGHT_ON, LIGHT_OFF, WAITING, SYS_OFF} SmartLightState;
typedef enum WaterLevelState {NORMAL, PRE_ALARM, ALARM, MANUAL} WaterLevelState;

#endif