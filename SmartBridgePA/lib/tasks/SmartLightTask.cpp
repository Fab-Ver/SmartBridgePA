#include "SmartLightTask.h"

#define TIME_OFF 5000

unsigned long lastDetection = 0;
unsigned long lastON = 0;

bool currDetectionState = false;
bool currDarkState = false;

SmartLightState currSmartLightState = SYS_ON;
SmartLightLedState currLed = LIGHT_OFF;
SmartLightLedState currSmartLightLedState  = LIGHT_OFF;

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
            bool detected = true;//motionSensor->isDetected();
            bool dark = true;//lightSensor->isDark();
            SmartLightState status;
            xSemaphoreTake(xMutex, portMAX_DELAY);
            status = currSmartLightState;
		    xSemaphoreGive(xMutex);
            /*-----------------------------*/
            if(currSmartLightState == SYS_ON){
                if(currLed == LIGHT_OFF){
                    if(detected && dark){
                        currLed = LIGHT_ON;
                    }
                    led->switchOff();
                } else if (currLed == LIGHT_ON){
                    lastON = millis();
                    if(!dark){
                        currLed = LIGHT_OFF;
                    } else if (dark && !detected){
                        currLed = WAITING;
                    }
                    led->switchOn();
                } else if (currLed == WAITING){
                    if(dark && detected){
                        currLed = LIGHT_ON;
                    } else {
                        if((millis() - lastON >= TIME_OFF) || !dark){
                            currLed = LIGHT_OFF;
                        }
                    }
                }
                xSemaphoreTake(xMutex, portMAX_DELAY);
                currSmartLightLedState = currLed;
                currDetectionState = detected;
                currDarkState = dark;
		        xSemaphoreGive(xMutex);
            } else {
                led->switchOff();
            }
        }
		vTaskDelay(500);
  	}
}
        
