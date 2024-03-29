#ifndef __SMART_LIGHT_TASK__
#define __SMART_LIGHT_TASK__

#include "Task.h"
#include "Led.h"
#include "LightSensor.h"
#include "MotionSensor.h"
#include "commons.h"

class SmartLightTask : public Task {
    private:
        Light* led;
        PhotoResistor* lightSensor;
        Pir* motionSensor; 
        TaskHandle_t SmartLightHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        SmartLightTask(int ledPin, int lsPin, int msPin);
        void init(); 
};

#endif