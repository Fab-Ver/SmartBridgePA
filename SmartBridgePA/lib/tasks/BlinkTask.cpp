#include "BlinkTask.h"

unsigned long lastBlink = 0;
BlinkState currBS = BLINK_OFF;

BlinkState currBlinkState = BLINK_OFF;

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
    for(;;){
		unsigned long now = millis();
        /*Local variables update*/
        xSemaphoreTake(xMutex, portMAX_DELAY);
		currBS = currWaterLevelState != PRE_ALARM ? BLINK_OFF : currBS == BLINK_OFF ? ON : currBS;
		xSemaphoreGive(xMutex);
        /*------------------------------*/
		if(now - lastBlink >= BLINK_PERIOD && currBS != BLINK_OFF){
			lastBlink = now;
			if(currBS == ON){
                led->switchOff();
                currBS = OFF;
            } else if (currBS == OFF){
                led->switchOn();
                currBS = ON;
            }
		} else if (now - lastBlink >= BLINK_PERIOD && currBS == BLINK_OFF){
            lastBlink = now;
            led->switchOff();
        }
        /*Shared variables update*/
        xSemaphoreTake(xMutex, portMAX_DELAY);
		currBlinkState = currBS;
		xSemaphoreGive(xMutex);
        /*--------------------------------*/
		vTaskDelay(500);
  	}
}