#ifndef __COMMUNICATION_TASK__
#define __COMMUNICATION_TASK__

#include "Task.h"
#include "commons.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"

/* MQTT subscribing callback */
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

class CommunicationTask : public Task{
    private:
        TaskHandle_t communicationHandle;
        void tick();
        static void tickWrapper(void* _this);
    public:
        CommunicationTask();
        void init();
};

#endif