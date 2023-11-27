#include "Valve.h"

Valve::Valve(int pin){
    this->pin = pin;
}

void Valve::setPosition(int angle){
    if(angle > 180){
        angle = 180;
    } else if(angle < 0){
        angle = 0;
    }
    float coeff = ((float)(MAX_PULSE_WIDTH - MIN_PULSE_WIDTH))/180.0;
    motor.write(MIN_PULSE_WIDTH + angle * coeff);
}

void Valve::on(){
    motor.attach(pin);
}

void Valve::off(){
    motor.detach();
}

