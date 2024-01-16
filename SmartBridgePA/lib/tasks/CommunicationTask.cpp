#include "CommunicationTask.h"
#include "util.h"

WiFiClient espClient;
PubSubClient client(espClient); 

/**
 * MQTT server address 
*/
const char* mqtt_server = "test.mosquitto.org";
const char* clientId = "smart-bridge";
/**
 * MQTT topic
*/
const char* wls_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/status";
const char* wl_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/distance";
const char* angle_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/angle";
const char* green_led_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/green";
const char* red_led_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/red";

const char* sl_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/status";
const char* dark_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/dark";
const char* detected_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/detected";

WaterLevelState currWLS = NORMAL;
WaterLevelState prevWLS = NORMAL;

ManualState currMS = MANUAL_OFF;
ManualState prevMS = MANUAL_OFF;

float currWL = WL1;
float prevWL = WL1;

int currAngle = 0;
int prevAngle = 0;

bool currGreenON = false;
bool prevGreenON = false;

bool currRedON = false;
bool prevRedON = false;

BlinkState currB = BLINK_OFF;
BlinkState prevB = BLINK_OFF;

SmartLightState currSL = LIGHT_OFF;
SmartLightState prevSL = LIGHT_OFF;

bool currDetection = false;
bool prevDetection = false;

bool currDark = false;
bool prevDark = false;


unsigned long lastCommunication = 0;

CommunicationTask::CommunicationTask(){
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void CommunicationTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"CommunicationTask",8192,this,1,&communicationHandle,0);
    delay(500);
}

void CommunicationTask::tickWrapper(void* _this){
	((CommunicationTask*)_this)->tick();
}

void CommunicationTask::tick(){
	for(;;){ 
    	unsigned long now = millis();
        if(now - lastCommunication >= COMMUNICATION_PERIOD){
        	lastCommunication = now;
          	if(!client.connected()){
            	reconnect();
          	}
          	client.loop();
            
            xSemaphoreTake(xMutex, portMAX_DELAY);
            currWLS = currWaterLevelState;
            currMS = currManualState;
            currWL = currWaterLevel;
            currAngle = currValveAngle;
            currGreenON = currGreenLedON;
            currRedON  = currRedLedON;

            currB = currBlinkState;
            currSL = currSmartLightState;
            currDetection = currDetectionState;
            currDark = currDarkState;
            xSemaphoreGive(xMutex);

          	if(currWLS != prevWLS){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["status"] = convertWaterLevelState(currWLS);
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(wls_topic,msg_json);
          		prevWLS = currWLS;
          		free(msg_json);
          	}

			if(currWL != prevWL){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["distance"] = currWL;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(wl_topic,msg_json);
          		prevWL = currWL;
          		free(msg_json);
          	}

			if(currAngle != prevAngle){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["angle"] = currAngle;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(angle_topic,msg_json);
          		prevAngle = currAngle;
          		free(msg_json);
          	}

			if(currGreenON != prevGreenON){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["on"] = currGreenON;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(green_led_topic,msg_json);
          		prevGreenON = currGreenON;
          		free(msg_json);
          	}

			if(currRedON != prevRedON || currB != prevB){
				DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["on"] = currRedON;
				doc["blinking"] = currB == BLINK_OFF ? false : true;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(red_led_topic,msg_json);
          		prevRedON = currRedON;
				prevB = currB;
          		free(msg_json);
			}

			if(currSL != prevSL){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:smart-light-subsystem";
          		doc["status"] = convertSmartLightState(currSL);
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(sl_topic,msg_json);
          		prevSL = currSL;
          		free(msg_json);
          	}

			if(currDetection != prevDetection){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:smart-light-subsystem";
          		doc["detected"] = currDetection;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(detected_topic,msg_json);
          		prevDetection = currDetection;
          		free(msg_json);
          	}

            if(currDark != prevDark){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:smart-light-subsystem";
          		doc["dark"] = currDark;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		//Serial.println(msg_json);
          		client.publish(dark_topic,msg_json);
          		prevDark = currDark;
          		free(msg_json);
          	}
        }
        vTaskDelay(COMMUNICATION_PERIOD);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
	//Serial.println(String("Message arrived on [") + topic + "] len: " + length + "   " + String((char *)payload) + "\n");
  	/*StaticJsonDocument<200> doc;
  	DeserializationError error = deserializeJson(doc, );
		if(error){
			Serial.print(F("deserializeJson() failed: "));
    		Serial.println(error.c_str());
		}
	/*JsonObject json = doc.as<JsonObject>();
	if(json.containsKey("light")){
		bool light = doc["light"];
		LightState currLightState = light ? ON : OFF;
		if(currLightState != prevLightState){
			rollerBlind->off();
			if(currLightState == ON){
				led->switchOn();
			} else {
				led->switchOff();
			}
			prevLightState = currLightState;
			return true;
		}
	}
	if(json.containsKey("roller_blind")){
		int currRollerBlindState = doc["roller_blind"];
		if(currRollerBlindState != prevRollerBlindState){
			int angle = map(currRollerBlindState, 0, 100, 0, 180);
			rollerBlind->on();
			rollerBlind->setPosition(angle);
			prevRollerBlindState = currRollerBlindState;
			return true;
		}
	}*/
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*Attempt to connect*/
    if(client.connect(clientId)){
      Serial.println("connected");
      client.subscribe(wls_topic);
      client.subscribe(wl_topic);
	  client.subscribe(angle_topic);
	  client.subscribe(green_led_topic);
	  client.subscribe(red_led_topic);
	  client.subscribe(sl_topic);
    } else {
      Serial.print("failed rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds.");
      delay(5000);
    }
  }
}