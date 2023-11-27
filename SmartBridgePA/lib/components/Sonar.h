#ifndef __SONAR__
#define __SONAR__

class Sonar {
    public:
        /*
            Return the current distance from an object. 
        */
        virtual float getDistance() = 0;
};

#endif