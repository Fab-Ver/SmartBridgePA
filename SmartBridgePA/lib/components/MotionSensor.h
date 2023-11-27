#ifndef __MOTION_SENSOR__
#define __MOTION_SENSOR__

#include "Pir.h"
#define CALIBRATION_TIME_SEC 20

class MotionSensor : public Pir{
    private: 
        int pin;
    public:
        MotionSensor(int pin);
        bool isDetected();
};

#endif