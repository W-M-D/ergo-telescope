#ifndef CERGO_GPS_H
#define CERGO_GPS_H
#define UBX_MAX_SIZE 60

#include <inttypes.h>
#include <string.h>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <deque>
#include <forward_list>
#include "CLog.h"
#include "CERGO_SERIAL.h"

class CERGO_GPS
{
public:
    CERGO_GPS(int,std::string );


    ///**************************************///
    ///UBLOX TOOLS

    ///**********************************///
    ///COMS
    int Read_data(std::deque <uint8_t> &); // takes in a data list from the serial function then tests and parses the data.
    int ubx_checksum(std::deque <uint8_t> &);//adds the current UBLOX DATA to the checksums described here http://www.u-blox.com/images/downloads/Product_Docs/u-blox6_ReceiverDescriptionProtocolSpec_%28GPS.G6-SW-10018%29.pdf
    void sendUBX(int *MSG, int len); // sends message to UBLOX ???
    bool getUBX_ACK(int *MSG);
    ///**********************************///

    int parse_ubx_gps(std::deque <uint8_t> & data_list);//parses the UBLOX DATA by checking the ID ETC Depends on *_bytes
    ///**************************************///


    ///**************************************///
    ///BYTE TOOLS **
    long join_4_bytes(std::deque <uint8_t> & data_list); // Joins four bytes together
    int join_2_bytes(std::deque <uint8_t> & data_list); // joins two bytes together
    uint8_t one_byte(std::deque <uint8_t> & data_list); // takes a single byte from buffer returns long
    char* bytes_to_decimal(char Buffer[]); //converts bytes to decimal  unimplemented
    ///**************************************///


    ///**************************************///
    ///PACKET TOOLS
    //DEPENDS ON : GpsToMjd,MjdToDate,GPSTOWtoUTC,*_packatize,
    void packet_manager(std::forward_list <std::string> & string_list);
    bool packet_adder();
    std::string packatize(); //takes data and puts it into a string that is encoded for server use
    std::string URLEncode(const char* msg); // encodes the server string
    std::string cordinate_packatize(long  cord, int); //Function to packatize the coordinate data lat/long/alt
    std::string nanosecond_packatize(long  nS);//Function to packatize the nanosecond data
    std::string date_packatize(); // Function to packatize the date

    ///**************************************///
    ///TIME TOOLS
    long GpsToMjd (long , long  ); // takes GpsWeek,GpsSecond and returns a long Modified Julian Day
    long DateToMjd (long , long , long ); // takes in year month and day and returns a Modified Julian Day
    void MjdToDate (long &, long &, long & , long &);  // Takes a Modified Julian Day and converts it to a date in YMD format called by the packatize function
    std::string GPSTOWtoUTC(long secs);     //converts the GPS Time of the Week presented by the ERGO GPS called by packatize
    double diffclock (long clock1,long clock2); // takes two clocks and returns the time btween them in mS


    ///**************************************///

    ///**************************************///
    ///VAR GET TOOLS







    virtual ~CERGO_GPS();
protected:
private:

    long CURRENT_CENTURY;
    CLog * Log;
    std::string unitid;
    std::forward_list <std::string> packet_list;
    CERGO_SERIAL * Serial; 
    int DEBUG_LEVEL;
    bool NewData;
    bool pos_data;
    uint8_t gps_fix;
    int gps_sat_numbers;
    uint8_t ch;//marker channel 0 or 1
    uint8_t flags;//Bitmask
    int counter ;
    int wnR ;
    int wnF ;
    long towMsR;
    long  towSubMsR;
    long towMsF;
    long towSubMsF;
    long accEst;
    int checksum;
    long Time ; // ms Time of week
    long Longitude; // lon*10000000
    long Lattitude ; // lat*10000000
    long Altitude;  // elipsoid heigth mm
};

#endif // CERGO_GPS_H
