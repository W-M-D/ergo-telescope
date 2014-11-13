#ifndef CERGO_SERIAL_H
#define CERGO_SERIAL_H
#define SERIAL 3
#define GPIO_MODE_PATH "/sys/devices/virtual/misc/gpio/mode/"
#define GPIO_FILENAME "gpio"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "CLog.h"
#include <forward_list>
#include <deque>
#include <sys/poll.h>
#include <sys/ioctl.h>


class CERGO_SERIAL
{
public:

    CERGO_SERIAL(int);
    CERGO_SERIAL();

    bool  serial_init(int);
    void serial_setup(int);
    bool getUBX_ACK(int *);

    void sendUBX(int *,size_t);

    int data_read (std::deque <uint8_t> &);
    void setPinMode(int pinID, int mode);
    void writeFile(int fileID, int value);




    virtual ~CERGO_SERIAL();
protected:

private:
    CERGO_SERIAL * Serial;
    int v_time;
    int tty_fd;
    struct termios tio;
    uint8_t data_in;
    int DEBUG_LEVEL;

    // struct termios port_options;

    CLog * Log;




};

#endif // CERGO_SERIAL_H


