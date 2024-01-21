#ifndef __CONFIG__
#define __CONFIG__

#define BLINK_PERIOD 2000
#define SMART_LIGHT_PERIOD 200
#define WATER_LEVEL_PERIOD 200
#define COMMUNICATION_PERIOD 2000

#define WL1 70
#define WL2 40
#define WL_MAX 2

#define TRIG_PIN 37 
#define ECHO_PIN 36
#define VALVE_PIN 10
#define GREEN_PIN 5
#define RED_PIN 18 

#define LED_PIN 6 
#define LS_PIN 3
#define MS_PIN 46

typedef enum BlinkState {ON, OFF, BLINK_OFF} BlinkState;
typedef enum SmartLightState {SYS_OFF, SYS_ON} SmartLightState;
typedef enum SmartLightLedState {LIGHT_ON, LIGHT_OFF, WAITING} SmartLightLedState;
typedef enum WaterLevelState {NORMAL, PRE_ALARM, ALARM} WaterLevelState;
typedef enum ManualState {MANUAL_ON, MANUAL_OFF} ManualState;

#endif