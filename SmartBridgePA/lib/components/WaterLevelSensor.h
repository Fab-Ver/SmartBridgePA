#ifndef __WATER_LEVEL_SENSOR__
#define __WATER_LEVEL_SENSOR__

#include "Sonar.h"

class WaterLevelSensor : public Sonar {
    private:
        int trigPin, echoPin;
    public:
        WaterLevelSensor(int trigPin, int echoPin);
        float getDistance();
};

#endif