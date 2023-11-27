#include "MotionSensor.h"
#include <Arduino.h>

MotionSensor::MotionSensor(int pin){
    this->pin = pin;
    pinMode(pin,INPUT);
    Serial.print("Calibrating motion sensor");
    for(int i = 0; i < CALIBRATION_TIME_SEC; i++){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("done.");
    delay(50);
}

bool MotionSensor::isDetected(){
    return digitalRead(pin);
}