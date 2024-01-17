#include "CommunicationTask.h"
#include "util.h"

WiFiClient espClient;
PubSubClient client(espClient); 


const char* mqtt_server = "test.mosquitto.org"; /* MQTT server address */
const char* clientId = "smart-bridge"; 

/**
 * Water Level Task MQTT topic
*/
const char* wls_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/status";
const char* wl_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/distance";
const char* angle_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/angle";
const char* green_led_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/green";
const char* red_led_topic = "subsystems/org.eclipse.ditto:water-level-subsystem/red";

/**
 * Smart Light Task MQTT topic
*/
const char* dark_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/dark";
const char* detected_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/detected";
const char* light_topic = "subsystems/org.eclipse.ditto:smart-light-subsystem/light";

/**
 * External (Ditto DT) MQTT topic
*/
const char* slsExternal = "subsystems/downlink/org.eclipse.ditto:smart-light-subsystem";
const char* wlsExternal = "subsystems/downlink/org.eclipse.ditto:water-level-subsystem";

WaterLevelState currWLS = NORMAL;
WaterLevelState prevWLS = NORMAL;

ManualState currMS = MANUAL_OFF;
ManualState prevMS = MANUAL_OFF;

String currWL = "0.00";
String prevWL = "0.00";

int currAngle = 0;
int prevAngle = 0;

bool currGreenON = false;
bool prevGreenON = false;

bool currRedON = false;
bool prevRedON = false;

BlinkState currB = BLINK_OFF;
BlinkState prevB = BLINK_OFF;

bool currDetection = false;
bool prevDetection = false;

bool currDark = false;
bool prevDark = false;

SmartLightLedState currLedState = LIGHT_OFF;
SmartLightLedState prevLedState = LIGHT_OFF;

unsigned long lastCommunication = 0;

CommunicationTask::CommunicationTask(){
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
	client.setBufferSize(2048);
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
            currWL = String(currWaterLevel,2);
            currAngle = currValveAngle;
            currGreenON = currGreenLedON;
            currRedON  = currRedLedON;
			currB = currBlinkState;
            currDetection = currDetectionState;
            currDark = currDarkState;
			currLedState = currSmartLightLedState;
            xSemaphoreGive(xMutex);

          	if(currWLS != prevWLS){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:water-level-subsystem";
          		doc["status"] = convertWaterLevelState(currWLS);
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
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
          		client.publish(red_led_topic,msg_json);
          		prevRedON = currRedON;
				prevB = currB;
          		free(msg_json);
			}

			if(prevLedState != currLedState){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:smart-light-subsystem";
          		doc["on"] = currLedState == LIGHT_OFF ? false : true;
				doc["waiting"] = currLedState == WAITING ? true : false;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
          		client.publish(light_topic,msg_json);
				prevLedState = currLedState;
          		free(msg_json);
          	}

			if(currDetection != prevDetection){
              	DynamicJsonDocument doc(1024);
				doc["thingId"] = "org.eclipse.ditto:smart-light-subsystem";
          		doc["detected"] = currDetection;
          		char* msg_json = (char*) malloc(1024);
          		serializeJson(doc,msg_json,1024);
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
          		client.publish(dark_topic,msg_json);
          		prevDark = currDark;
          		free(msg_json);
          	}
        }
        vTaskDelay(COMMUNICATION_PERIOD);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
	
	if(strcmp(topic,"subsystems/downlink/org.eclipse.ditto:smart-light-subsystem") == 0){
		Serial.println(String("Message arrived on [") + topic + "] len: " + length + "   " + String((char *)payload) + "\n");
  		StaticJsonDocument<750> doc;
  		DeserializationError error = deserializeJson(doc, payload);
		if(error){
			Serial.print(F("deserializeJson() failed: "));
    		Serial.println(error.c_str());
			return;
		}
		JsonObject json = doc.as<JsonObject>();
		if(json.containsKey("path") && json.containsKey("value")){
			String path = doc["path"];
			String value = doc["value"];		
			if(path == "/features/status/properties/status"){
				xSemaphoreTake(xMutex, portMAX_DELAY);
                currSmartLightState = value == "SYS_OFF" ? SYS_OFF : SYS_ON;
		    	xSemaphoreGive(xMutex);
			}
		} else {
			Serial.println("[error] - Invalid JSON Object.");
		}
	}
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*Attempt to connect*/
    if(client.connect(clientId)){
      Serial.println("connected");
	  client.subscribe(wlsExternal);
	  client.subscribe(slsExternal);
    } else {
      Serial.print("failed rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds.");
      delay(5000);
    }
  }
}