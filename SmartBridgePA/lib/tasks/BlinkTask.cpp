#include "BlinkTask.h"

BlinkTask::BlinkTask(int ledPin){
    this->led = new Led(ledPin);
}

void BlinkTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"BlinkTask",8192,this,1,&BlinkHandle,1);
    delay(500);
}

void BlinkTask::tickWrapper(void* _this){
    ((BlinkTask*)_this)->tick();
}

void BlinkTask::tick(){

}