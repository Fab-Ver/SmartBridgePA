#include "ValveKnob.h"

ValveKnob::ValveKnob(int pin){
    this->pin = pin;
}

int ValveKnob::getAngle(){
    int value = analogRead(pin);
    return map(value, 0, 4095, 0, 180);
}