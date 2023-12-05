#ifndef __BLINK_TASK__
#define __BLINK_TASK__

#include "Task.h"
#include "Led.h"
#include <Arduino.h>
#include "config.h"
#include "commons.h"

class BlinkTask : public Task {
    private:
        Light* led;
        TaskHandle_t BlinkHandle;
        BlinkState currBlinkState;
        void tick();
        static void tickWrapper(void* _this);
    public:
        BlinkTask(int ledPin);
        void init(); 
};

#endif