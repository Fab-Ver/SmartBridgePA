#include "Valve.h"

Valve::Valve(int pin){
    this->pin = pin;
}

void Valve::setPosition(int angle){
    motor.write(angle);
}

void Valve::on(){
    if(!motor.attached()){
        motor.setPeriodHertz(50);
        motor.attach(pin);
    }
}

void Valve::off(){
    if(motor.attached()){
        motor.detach();
    }
}

