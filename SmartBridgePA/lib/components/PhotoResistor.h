#ifndef __PHOTO_RESISTOR__
#define __PHOTO_RESISTOR__

class PhotoResistor{
    public:
        /**
         * Report the light status of the bridge.
         * @return true if the bridge is dark, false otherwise. 
        */
        virtual bool isDark() = 0;
};

#endif