   #include "LightSensor.h"
#include <Arduino.h>

LightSensor::LightSensor(int pin){
    this->pin = pin;
}

bool LightSensor::isDark(){
    int value = analogRead(pin);
    return value < THRESHOLD;
}