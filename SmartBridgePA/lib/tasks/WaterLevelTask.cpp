#include "WaterLevelTask.h"

unsigned long lastDetection = 0;

float currWaterLevel = 0.0;
int currValveAngle = 0;
bool currGreenLedON = false;
bool currRedLedON = false;

WaterLevelState currTaskState = NORMAL;
WaterLevelState prevTaskState = NORMAL;
ManualState currManualState = MANUAL_OFF;

void switchState(float currWL);

WaterLevelTask::WaterLevelTask(int trigPin, int echoPin, int valvePin, int greenPin, int redPin, int knobPin){
    this->greenLed = new Led(greenPin);
    this->redLed = new Led(redPin);
    this->waterLevelSensor = new WaterLevelSensor(trigPin, echoPin);
    this->valve = new Valve(valvePin);
    this->knob = new ValveKnob(knobPin);
    this->currWaterLevelState = NORMAL;
}

void WaterLevelTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"WaterLevelTask",8192,this,1,&WaterLevelHandle,1);
    delay(500);
}

void WaterLevelTask::tickWrapper(void* _this){
    ((WaterLevelTask*)_this)->tick();
}

void WaterLevelTask::tick(){
    for(;;){
		unsigned long now = millis();
        if(now - lastDetection >= WATER_LEVEL_PERIOD){
            lastDetection = now;
            ManualState manual;
            int manualAngle;
            xSemaphoreTake(xMutex, portMAX_DELAY);
	        manual = currManualState;
	        xSemaphoreGive(xMutex);

            float currWL = waterLevelSensor->getDistance();
            int valveAngle;
            bool greenON;
            bool redON;

            if(manual == MANUAL_OFF){
                switchState(currWL);
            } else {
                manualAngle = knob->getAngle();
            }

            switch(currTaskState){
                case NORMAL:{
                    if(currTaskState != prevTaskState){
                        if(prevTaskState == ALARM || prevTaskState == MANUAL){
                            valve->off();
                        }
                        greenLed->switchOn();
                        redLed->switchOff();
                        valveAngle = 0;
                        greenON = true;
                        redON = false;
                    }
                }break;
                case PRE_ALARM:{
                    if(currTaskState != prevTaskState){
                        if(prevTaskState == ALARM || prevTaskState == MANUAL){
                            valve->off();
                        }
                    greenLed->switchOff();
                    valveAngle = 0;
                    greenON = false;
                    redON = true;
                     }
                }break;
                case ALARM:{
                    int angle = map(currWL*100,WL_MAX,WL2,180,0);
                    if(currTaskState != prevTaskState){
                        redLed->switchOn();
                        greenLed->switchOff();
                        if(prevTaskState != MANUAL){
                            valve->on();
                        }
                        redON = true;
                        greenON = false;
                    }
                    valve->setPosition(angle);
                    valveAngle = angle;
                } break;
                case MANUAL: {
                    if(currTaskState != prevTaskState){
                        redLed->switchOn();
                        greenLed->switchOff();
                        if(prevTaskState != ALARM){
                            valve->on();
                        }
                        redON = true;
                        greenON = false;
                    }
                    valve->setPosition(manualAngle);
                    valveAngle = manualAngle;
                }break;
            }

            /*Shared variables update*/
            xSemaphoreTake(xMutex, portMAX_DELAY);
	        currWaterLevelState = currTaskState;
            currWaterLevel = currWL;
            currValveAngle = valveAngle;
            currGreenLedON = greenON;
            currRedLedON = redON;
	        xSemaphoreGive(xMutex);
            /*---------------------*/
        }
        vTaskDelay(500);
  	}
}

void switchState(float currWL){
    prevTaskState = currTaskState;
    currWL = currWL * 100;
    if(currWL > WL1){
        currTaskState = NORMAL;
    } else if (currWL <= WL1 && currWL > WL2){
        currTaskState = PRE_ALARM;
    } else if (currWL <= WL2 && currWL >= WL_MAX){
        currTaskState = ALARM;
    }
}