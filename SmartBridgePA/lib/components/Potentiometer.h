#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Potentiometer {
    public:
        /*
            Get the current angle, mapped according potentiometer value. 
        */
        virtual int getAngle() = 0;
};

#endif