#ifndef __VALVE__
#define __VALVE__

#include "ServoMotor.h"
#include <Arduino.h>
#include "ESP32Servo.h"

class Valve : public ServoMotor{
    private:
        int pin;
        Servo motor;
    public:
        Valve(int pin);
        void on();
        void off();
        void setPosition(int angle);
};
    
#endif