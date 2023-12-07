#ifndef __BLINK_TASK__
#define __BLINK_TASK__

#include "Task.h"
#include "Led.h"
#include "commons.h"

class BlinkTask : public Task {
    private:
        Light* led;
        TaskHandle_t BlinkHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        BlinkTask(int ledPin);
        void init(); 
};

#endif