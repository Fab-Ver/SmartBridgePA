#ifndef __SERVO_MOTOR__
#define __SERVO_MOTOR__

class ServoMotor {
    public:
        /**
         * Turn on the servo motor. 
        */
        virtual void on() = 0;

        /**
         * Turn off the servo motor. 
        */
        virtual void off() = 0;
        
        /**
         * Set position of the servo motor to the given angle.
         * @param angle the motor adjustment angle. 
        */
        virtual void setPosition(int angle) = 0;
};

#endif