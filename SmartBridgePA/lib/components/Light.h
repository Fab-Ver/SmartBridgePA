#ifndef __LIGHT__
#define __LIGHT__

class Light {
    public:
    /**
     * Switch on the light.
    */
    virtual void switchOn() = 0;

    /**
     * Switch off the light. 
    */
    virtual void switchOff() = 0;
};

#endif