#include "WaterLevelTask.h"

WaterLevelTask::WaterLevelTask(int trigPin, int echoPin, int valvePin, int greenPin, int redPin){
    this->greenLed = new Led(greenPin);
    this->redLed = new Led(redPin);
    this->waterLevelSensor = new WaterLevelSensor(trigPin, echoPin);
    this->valve = new Valve(valvePin);
    //pot = new PotentiometerImpl(potPin);
}

void WaterLevelTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"WaterLevelTask",8192,this,1,&WaterLevelHandle,1);
    delay(500);
}

void WaterLevelTask::tickWrapper(void* _this){
    ((WaterLevelTask*)_this)->tick();
}

void WaterLevelTask::tick(){

}