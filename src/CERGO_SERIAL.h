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
#include <vector>

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
    
    int read_config_file(std::string file_name);
    int parse_config_file_line(std::string & raw_line,std::deque <uint8_t> & command_hex_data);
    int generate_checksum(std::deque <uint8_t> &); 
    int send_config(std::string name, std::deque <uint8_t>);


    bool is_empty(std::ifstream& data_in)
    {
	return data_in.peek() == std::ifstream::traits_type::eof();
    }

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
    struct ublox_command
    {
      std::string command_name;
      uint8_t CMD_MSG[];
    };
    
    CLog * Log;




};

#endif // CERGO_SERIAL_H


