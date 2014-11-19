#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "CERGO_SERIAL.h"
static const char* portName = "/dev/ttyAMA0";

CERGO_SERIAL::CERGO_SERIAL(int debug_level)
{
    DEBUG_LEVEL = debug_level;
    if(DEBUG_LEVEL >= 1)
    {

        Log->add("serial port %d \n\n",tty_fd);

    }
    v_time = 5;
    while(!serial_init(9600))
    {
        sleep(60);
    }

}


bool CERGO_SERIAL::serial_init(int baud)
{
    if(tty_fd == 3)
    {
        close(tty_fd);
        usleep( 1 * 1000 );

        if(DEBUG_LEVEL >= 3)
        {
            Log->add(" reset TTY_FD = %d",tty_fd);
        }
    }
    else
    {
        if(DEBUG_LEVEL >= 3)
        {
            Log->add("TTY_FD = %d",tty_fd);
        }
    }

    tcgetattr(tty_fd, &tio);
    tcflush(tty_fd, TCIOFLUSH);

    fcntl(STDIN_FILENO, F_SETFL, 0);       // make the reads non-blocking
    memset(&tio,0,sizeof(tio));

    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;
    tio.c_cflag &= ~PARENB;// no parity
    tio.c_cflag &= ~CSTOPB;// one stop bit
  
    tio.c_lflag=0;

<<<<<<< HEAD
    tio.c_cc[VMIN]=30;
    tio.c_cc[VTIME]=v_time;
=======
    tio.c_cc[VMIN]=40;
    tio.c_cc[VTIME]=10;
>>>>>>> 322adb21e9de417a2888ba093752d4303e8e19aa



    tty_fd=open(portName, O_RDWR );

    if (baud == 9600)
    {
        cfsetospeed(&tio,B9600);            // 115200 baud
        cfsetispeed(&tio,B9600);            // 115200 baud
        usleep( 3 * 1000 );
    }

    if (baud == 38400)
    {
        cfsetospeed(&tio,B38400);            // 115200 baud
        cfsetispeed(&tio,B38400);            // 115200 baud

        usleep( 3 * 1000 );
    }

    tcsetattr(tty_fd,TCSANOW,&tio);
    tcflush(tty_fd, TCIOFLUSH);
    fcntl(tty_fd, F_SETFL, 0);

    usleep( 1 * 1000 );

    return true;
}


int CERGO_SERIAL::data_read (std::deque <uint8_t> & data_list)
{
    struct pollfd fds[1];
    fds[0].fd = tty_fd;
    fds[0].events = POLLIN ;
    int pollrc = poll( fds, 1,10);
    if (pollrc < 0)
    {
        perror("poll");
    }
    else if( pollrc > 0)
    {
        if( fds[0].revents )
        {
            if( POLLIN)
            {
                int bytes_avail = 0;
                ioctl(tty_fd, FIONREAD, &bytes_avail);
                for(int i = 0; i < bytes_avail; i++)
                {
                    uint8_t read_byte = 0;
                    read(tty_fd, &read_byte,1);
                    data_list.emplace_back(read_byte);
                }
            }
        }
    }
    return -1;
}



void CERGO_SERIAL::serial_setup(int ID)

{

    if (ID == 1337)
    {
        int CFG_PRT[] =                {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x54};
        int CFG_RATE[] =              {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};
        int CFG_PRT2[] =              {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0xCE };
        int CFG_TM2[] =               {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x0D, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x25};
        int CFG_NAV_POSLLH[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x13, 0xBE };
        int CFG_NAV_SOL[]=        {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x17, 0xDA};




        sendUBX(CFG_PRT,(sizeof(CFG_PRT)/sizeof(int)));
        Log->add("Sucess:CFG_PRT %s" ,getUBX_ACK(CFG_PRT) ? "true" : "false");

        serial_init(38400);

        sendUBX(CFG_RATE,(sizeof(CFG_RATE)/sizeof(int)));
        Log->add("Sucess:CFG_RATE %s" ,getUBX_ACK(CFG_RATE) ? "true" : "false");

        sendUBX(CFG_PRT2,(sizeof(CFG_PRT2)/sizeof(int)));
        Log->add("Sucess:CFG_PRT2 %s" ,getUBX_ACK(CFG_PRT2) ? "true" : "false");


        sendUBX(CFG_TM2,(sizeof(CFG_TM2)/sizeof(int)));

        Log->add("Sucess:CFG_TM2 %s" ,getUBX_ACK(CFG_TM2) ? "true" : "false");


        sendUBX(CFG_NAV_POSLLH,(sizeof(CFG_NAV_POSLLH)/sizeof(int)));
        Log->add("Sucess:CFG_NAV_POSLLH %s" ,getUBX_ACK(CFG_NAV_POSLLH) ? "true" : "false");


        sendUBX(CFG_NAV_SOL,(sizeof(CFG_NAV_SOL)/sizeof(int)));
        Log->add("Sucess:CFG_NAV_SOL %s" ,getUBX_ACK(CFG_NAV_SOL) ? "true" : "false");

    }

    if (ID == 247)
    {
        int CFG_NAV_POSLLH[] = {0xB5, 0x62, 0x06, 0x03, 0x1C, 0x00, 0x06, 0x03, 0x10, 0x18, 0x14, 0x05, 0x00, 0x3C, 0x3C, 0x14, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x17, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x0F, 0x00, 0x00, 0x00, 0x91, 0x54};
        sendUBX(CFG_NAV_POSLLH,sizeof(CFG_NAV_POSLLH));


        Log->add("Sucess: %s" ,getUBX_ACK(CFG_NAV_POSLLH) ? "true" : "false");
    }

}



void CERGO_SERIAL::sendUBX(int *MSG,size_t len)
{

    lseek(tty_fd, 0, SEEK_SET);

    if(DEBUG_LEVEL >= 3)
    {

        Log->add("\n\nSENDING BYTES:\n");

    }
    for(unsigned int i =0; i < len; i++)
    {
        uint8_t c = MSG[i];

        if(POLLOUT)
        {

            write(tty_fd,&c,1);

            if(DEBUG_LEVEL >= 3)
            {

                Log->add("0x%X ",MSG[i]);

            }
        }
        else
        {

            i--;

        }
    }

    if(DEBUG_LEVEL >= 3)

    {
        Log->add("\n\n\n\n");

    }
}



bool CERGO_SERIAL::getUBX_ACK(int *MSG)
{
    int serial_timeout = 0;
    int ackByteID = 0;
    int ackPacket[10];
    clock_t start_clock = clock();

    std::deque <uint8_t> data_list; // list to store serial data

    if(DEBUG_LEVEL >= 2)
    {
        Log->add(" * Reading ACK response: ");
    }
    // Construct the expected ACK packet
    ackPacket[0] = 0xB5;	// header CPOCMSG[i]MSG[i]MSG[i]C
    ackPacket[1] = 0x62;	// header
    ackPacket[2] = 0x05;	// class
    ackPacket[3] = 0x01;	// id
    ackPacket[4] = 0x02;	// length
    ackPacket[5] = 0x00;
    ackPacket[6] = MSG[2];	// ACK class
    ackPacket[7] = MSG[3];	// ACK id
    ackPacket[8] = 0;		// CK_A
    ackPacket[9] = 0;		// CK_B

    // Calculate the checksums
    for (int i=2; i<8; i++)
    {
        ackPacket[8] = ackPacket[8] + ackPacket[i];
        ackPacket[9] = ackPacket[9] + ackPacket[8];
    }


    while (true)
    {
        data_read(data_list);
        // Test for success
        if (ackByteID > 9)
        {
            // All packets in order!

            if(DEBUG_LEVEL >= 2)

            {
                Log->add(" (SUCCESS!)");

            }
            return true;
        }

        // Make sure data is available to read
        if (((clock() - start_clock)/( CLOCKS_PER_SEC / 1000 )) > serial_timeout)
        {

            if(DEBUG_LEVEL >= 2)

            {

                Log->add(" (FAILED!)");

            }
            return false;
        }

        // Check that bytes arrive in sequence as per expected ACK packet

        while(!data_list.empty())

        {

            if(ackByteID > 9)
            {
                break;
            }

            if (data_list.front() == ackPacket[ackByteID])
            {
                ackByteID++;
            }
            else
            {
                ackByteID = 0;	// Reset and look again, invalid order
            }



            if(DEBUG_LEVEL >= 3)

            {

                Log->add("0x%X ",data_list.front());

            }


            data_list.pop_front();
        }

    }


}




CERGO_SERIAL::~CERGO_SERIAL()
{
    Log->add("closing %d",tty_fd);
    close(tty_fd);
}

