#ifndef __TASK__
#define __TASK__

class Task {
    private:
        virtual void tick() = 0;
        static void tickWrapper(void* _this);
    public: 
        virtual void init() = 0;    
};

#endif