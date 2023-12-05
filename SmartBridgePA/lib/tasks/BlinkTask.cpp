#include "BlinkTask.h"

unsigned long lastBlink = 0;

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
        xSemaphoreTake(xMutex, portMAX_DELAY);
		currBlinkState = currWaterLevelState != PRE_ALARM ? BLINK_OFF : currBlinkState == BLINK_OFF ? ON : currBlinkState;
		xSemaphoreGive(xMutex);
		if(now - lastBlink >= BLINK_PERIOD && currBlinkState != BLINK_OFF){
			lastBlink = now;
			if(currBlinkState == ON){
                led->switchOff();
                currBlinkState = OFF;
            } else if (currBlinkState == OFF){
                led->switchOn();
                currBlinkState = ON;
            }
		} else if (now - lastBlink >= BLINK_PERIOD && currBlinkState == BLINK_OFF){
            lastBlink = now;
            led->switchOff();
        }
		vTaskDelay(500);
  	}
}