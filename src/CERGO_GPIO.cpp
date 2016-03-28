#include "CERGO_GPIO.h"

CERGO_GPIO::CERGO_GPIO()
{
    export_gpio(GREEN_LEFT);//LEFT
    export_gpio(YELLOW_MIDDLE);//MIDDLE
    export_gpio(RED_RIGHT);//RIGHT CLOSEST TO GEIGER COUNTER
}

int CERGO_GPIO::export_gpio(int gpionum)
{
    std::string export_str = "/sys/class/gpio/export";
    std::ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (!exportgpio.is_open())
    {
        return -1;
    }

    exportgpio << gpionum ; //write GPIO number to export
    exportgpio.close(); //close export file
    setdir_gpio(gpionum);
    return 0;
}
void CERGO_GPIO::test_lights()
{
  std::chrono::milliseconds LIGHT_TIMER (200);
  
  for(int i = 0; i < 10; i++)
  {
    pulse_light(LIGHT_TIMER,GREEN_LEFT);
    pulse_light(LIGHT_TIMER,YELLOW_MIDDLE);
    pulse_light(LIGHT_TIMER,RED_RIGHT);
  }
}


int CERGO_GPIO::unexport_gpio(int gpionum)
{
    setval_gpio(0,gpionum);
    std::string unexport_str = "/sys/class/gpio/unexport";
    std::ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if(!unexportgpio.is_open())
    {
        return -1;
    }

    unexportgpio << gpionum ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;
}


int CERGO_GPIO::setdir_gpio(int gpionum)
{
    std::string direction = "out";
    std::stringstream setdir_str;
    setdir_str << "/sys/class/gpio/gpio" << gpionum << "/direction";
    std::ofstream setdirgpio(setdir_str.str().c_str()); // open direction file for gpio
    if(!setdirgpio.is_open())
    {
	exportgpio(gpionum);
	setdir_gpio(gpionum);
        return -1;
    }

    setdirgpio << direction; //write direction to direction file
    setdirgpio.close(); // close direction file
    return 0;
}

int CERGO_GPIO::setval_gpio(int val,int gpionum)
{

    std::stringstream setval_str;
    setval_str << "/sys/class/gpio/gpio" << gpionum << "/value";
    std::ofstream setvalgpio(setval_str.str()); // open value file for gpio
    if(!setvalgpio.is_open()) 
    {
        return -1;
    }

    setvalgpio << val ;//write value to value file
    setvalgpio.close();// close value file
    return 0;
}

void CERGO_GPIO::pulse_light(std::chrono::milliseconds dura,int light_name)
{
    setval_gpio(true,light_name);
    std::this_thread::sleep_for(dura);
    setval_gpio(false,light_name);
}



int CERGO_GPIO::getval_gpio(int gpionum)
{
    std::string val;
    std::stringstream getval_str;
    getval_str << "/sys/class/gpio/gpio" << gpionum << "/value";
    std::ifstream getvalgpio(getval_str.str());// open value file for gpio
    if (getvalgpio.is_open())
    {
        return -1;
    }

    getvalgpio >> val ;  //read gpio value
    getvalgpio.close(); //close the value file

    if(val != "0")
        return 1;
    else
        return 0;

    return -1;
}



CERGO_GPIO::~CERGO_GPIO()
{
    unexport_gpio(GREEN_LEFT);
    unexport_gpio(YELLOW_MIDDLE);
    unexport_gpio(RED_RIGHT);
}
