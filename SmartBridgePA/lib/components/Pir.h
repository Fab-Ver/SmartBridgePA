#ifndef __PIR__
#define __PIR__

class Pir{
    public:
        /*
            Return true if a person is detected, false otherwise. 
        */
        virtual bool isDetected() = 0;
};

#endif