#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "WaterLevelTask.h"
#include "SmartLightTask.h"
#include "BlinkTask.h"
#include "CommunicationTask.h"
#include "commons.h"

/**
 * WIFI network info
 * Insert your network info to use the program
*/
const char* ssid = "WIFI_name";
const char* password = "WIFI_password";

SemaphoreHandle_t xMutex;
void setup_wifi();

void setup() {
  Serial.begin(115200);
  xMutex = xSemaphoreCreateMutex();
  setup_wifi();
  Task* waterLevelTask = new WaterLevelTask(TRIG_PIN, ECHO_PIN, VALVE_PIN, GREEN_PIN, RED_PIN);
  Task* smartLightTask = new SmartLightTask(LED_PIN, LS_PIN, MS_PIN);
  Task* blinkTask = new BlinkTask(RED_PIN);
  Task* communicationTask = new CommunicationTask();

  waterLevelTask->init();
  smartLightTask->init();
  blinkTask->init();
  communicationTask->init();
}

void loop() {
  delay(1000000);
}

void setup_wifi(){
  delay(10);

  Serial.print(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
}