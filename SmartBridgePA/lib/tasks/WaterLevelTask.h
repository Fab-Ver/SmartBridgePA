#ifndef __WATER_LEVEL_TASK__
#define __WATER_LEVEL_TASK__

#include "Task.h"
#include "Led.h"
#include "WaterLevelSensor.h"
#include "ValveKnob.h"
#include "Valve.h"
#include "commons.h"

class WaterLevelTask : public Task {
    private:
        Light* greenLed;
        Light* redLed;
        Sonar* waterLevelSensor;
        ServoMotor* valve;
        Potentiometer* knob;
        TaskHandle_t WaterLevelHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        WaterLevelTask(int trigPin, int echoPin, int valvePin, int greenPin, int redPin, int knobPin);
        void init(); 
};

#endif