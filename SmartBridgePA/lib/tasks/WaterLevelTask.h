#ifndef __WATER_LEVEL_TASK__
#define __WATER_LEVEL_TASK__

#include "Task.h"
#include "Led.h"
#include "WaterLevelSensor.h"
#include "Valve.h"
#include <Arduino.h>

class WaterLevelTask : public Task {
    private:
        Light* greenLed;
        Light* redLed;
        Sonar* waterLevelSensor;
        ServoMotor* valve;
        //Potentiometer* pot;
        TaskHandle_t WaterLevelHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        WaterLevelTask(int trigPin, int echoPin, int valvePin, int greenPin, int redPin /*int potPin,*/);
        void init(); 
};

#endif