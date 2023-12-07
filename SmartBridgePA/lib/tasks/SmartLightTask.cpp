#include "SmartLightTask.h"

#define TIME_OFF 5000

unsigned long lastDetection = 0;
unsigned long lastON = 0;

bool currDetectionState = false;
bool currDarkState = false;
SmartLightState currSLS = LIGHT_OFF;

SmartLightState currSmartLightState  = LIGHT_OFF;

SmartLightTask::SmartLightTask(int ledPin, int lsPin, int msPin){
    this->led = new Led(ledPin);
    this->lightSensor = new LightSensor(lsPin);
    this->motionSensor = new MotionSensor(msPin);
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
            /*Local variables update*/
            bool detected = motionSensor->isDetected();
            bool dark = lightSensor->isDark();
            xSemaphoreTake(xMutex, portMAX_DELAY);
		    currSLS = currWaterLevelState == ALARM ? SYS_OFF : currSLS == SYS_OFF ? LIGHT_ON : currSLS;
		    xSemaphoreGive(xMutex);
            /*-----------------------------*/
            switch (currSLS){
                case LIGHT_OFF: {
                    if(detected && dark){
                        currSLS = LIGHT_ON;
                    }
                    led->switchOff();
                }break;
                case LIGHT_ON:{
                    lastON = millis();
                    if(!dark){
                        currSLS = LIGHT_OFF;
                    } else if (dark && !detected){
                        currSLS = WAITING;
                    }
                    led->switchOn();
                }break;
                case WAITING:{
                    if(dark && detected){
                        currSLS = LIGHT_ON;
                    } else {
                        if((millis() - lastON >= TIME_OFF) || !dark){
                            currSLS = LIGHT_OFF;
                        }
                    }
                }break;
                case SYS_OFF:{
                    led->switchOff();
                    //currTaskState = LIGHT_OFF;
                }break;
            }
            /*Shared Variables update*/
            xSemaphoreTake(xMutex, portMAX_DELAY);
		    currSmartLightState = currSLS;
            currDetectionState = detected;
            currDarkState = dark;
		    xSemaphoreGive(xMutex);
            /*-----------------------------*/

        }
		vTaskDelay(500);
  	}
}
        
