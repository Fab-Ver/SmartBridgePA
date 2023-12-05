#include "SmartLightTask.h"

#define TIME_OFF 5000

unsigned long lastDetection = 0;
bool currDetectionState = false;
bool currDarkState = false;
unsigned long lastON = 0;

SmartLightTask::SmartLightTask(int ledPin, int lsPin, int msPin){
    this->led = new Led(ledPin);
    this->lightSensor = new LightSensor(lsPin);
    this->motionSensor = new MotionSensor(msPin);
    this->currSmartLightState = LIGHT_OFF;
}

void SmartLightTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"SmartLightTask",8192,this,1,&SmartLightHandle,1);
    delay(500);
}

void SmartLightTask::tickWrapper(void* _this){
    ((SmartLightTask*)_this)->tick();
}

void SmartLightTask::tick(){
    for(;;){
		unsigned long now = millis();
        if(now - lastDetection >= SMART_LIGHT_PERIOD){
            lastDetection = now;
            bool detected = motionSensor->isDetected();
            bool dark = lightSensor->isDark();
            xSemaphoreTake(xMutex, portMAX_DELAY);
		    currSmartLightState = currWaterLevelState == ALARM ? SYS_OFF : currSmartLightState == SYS_OFF ? LIGHT_ON : currSmartLightState;
            currDetectionState = detected;
            currDarkState = dark;
		    xSemaphoreGive(xMutex);
            switch (currSmartLightState){
                case LIGHT_OFF: {
                    if(detected && dark){
                        currSmartLightState = LIGHT_ON;
                    }
                    led->switchOff();
                }break;
                case LIGHT_ON:{
                    lastON = millis();
                    if(!dark){
                        currSmartLightState = LIGHT_OFF;
                    } else if (dark && !detected){
                        currSmartLightState = WAITING;
                    }
                    led->switchOn();
                }break;
                case WAITING:{
                    if(dark && detected){
                        currSmartLightState = LIGHT_ON;
                    } else {
                        if((millis() - lastON >= TIME_OFF) || !dark){
                            currSmartLightState = LIGHT_OFF;
                        }
                    }
                }break;
                case SYS_OFF:{
                    led->switchOff();
                    currSmartLightState = LIGHT_OFF;
                }break;
            }
        }
		vTaskDelay(500);
  	}
}
        
