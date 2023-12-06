#include "BlinkTask.h"

unsigned long lastBlink = 0;
BlinkState currTaskState = BLINK_OFF;

BlinkTask::BlinkTask(int ledPin){
    this->led = new Led(ledPin);
    this->currBlinkState = BLINK_OFF;
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
        /*Aggiornamento variabili locali*/
        xSemaphoreTake(xMutex, portMAX_DELAY);
		currTaskState = currWaterLevelState != PRE_ALARM ? BLINK_OFF : currTaskState == BLINK_OFF ? ON : currTaskState;
		xSemaphoreGive(xMutex);
        /*------------------------------*/
		if(now - lastBlink >= BLINK_PERIOD && currTaskState != BLINK_OFF){
			lastBlink = now;
			if(currTaskState == ON){
                led->switchOff();
                currTaskState = OFF;
            } else if (currTaskState == OFF){
                led->switchOn();
                currTaskState = ON;
            }
		} else if (now - lastBlink >= BLINK_PERIOD && currTaskState == BLINK_OFF){
            lastBlink = now;
            led->switchOff();
        }
        /*Aggiornamento variabili condivise*/
        xSemaphoreTake(xMutex, portMAX_DELAY);
		currBlinkState = currTaskState;
		xSemaphoreGive(xMutex);
        /*--------------------------------*/
		vTaskDelay(500);
  	}
}