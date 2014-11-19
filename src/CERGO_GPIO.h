#ifndef CERGO_GPIO_H
#define CERGO_GPIO_H
#include <CLog.h>
#include <thread>
#include <chrono>
class CERGO_GPIO
{
    public:
        CERGO_GPIO();

        int export_gpio(int);
        int unexport_gpio(int );
        int setdir_gpio(int gpionum);
        int setval_gpio(int val,int gpionum);
        int getval_gpio(int gpionum);
        void pulse_light(std::chrono::milliseconds,int light_name);

        enum light_colours_t{RED_RIGHT = 24,YELLOW_MIDDLE= 23,GREEN_LEFT=18};

        virtual ~CERGO_GPIO();
    protected:
    private:

};

#endif // CERGO_GPIO_H
