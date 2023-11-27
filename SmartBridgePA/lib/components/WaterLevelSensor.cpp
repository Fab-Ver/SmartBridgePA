#include "WaterLevelSensor.h"
#include <Arduino.h>

#define MIN_RANGE 0.02
#define MAX_RANGE 4

/*Speed of sound in a 20°C environment */
const double SOUND_SPEED = 331.45 + 0.62*20; 
float prevValue = MAX_RANGE;

WaterLevelSensor::WaterLevelSensor(int trigPin, int echoPin){
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float WaterLevelSensor::getDistance(){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    
    unsigned long time_micros = pulseInLong(echoPin, HIGH);
    if(time_micros == 0){
        /*Echo not received, we use the previous valid value*/
        return prevValue;
    }
    float value = (time_micros/(2.0*1000000.0))*SOUND_SPEED;
    prevValue = value;
    if(value <= MIN_RANGE){
        return MIN_RANGE;
    } else if (value >= MAX_RANGE){
        return MAX_RANGE;
    }
    return value; 
};