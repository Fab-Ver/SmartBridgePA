#ifndef __VALVE_KNOB__
#define __VALVE_KNOB__

#include "Potentiometer.h"
#include <Arduino.h>

class ValveKnob : public Potentiometer{   
    private:
        int pin;
    public:
        ValveKnob(int pin);
        int getAngle();
};

#endif