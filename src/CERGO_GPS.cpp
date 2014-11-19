#include "CERGO_GPS.h"
#include <cmath>
#include <unistd.h>

CERGO_GPS::CERGO_GPS(int debug_level)
{
    std::ifstream unit_id_file;
    unit_id_file.open("/etc/ERGO/unit_id");
    getline(unit_id_file,unitid);
    unit_id_file.close();
    Lattitude =0;
    Longitude = 0;
    Altitude = 0;
    DEBUG_LEVEL = debug_level;
}

///******************************************************************///
///UBLOX COMMS

int CERGO_GPS::Read_data(std::deque<uint8_t> & data_list )
{
    int test_int = 0;
    while(!data_list.empty())
    {
        if(data_list.front() == 0xB5)//check if front of list == 0xB5
        {
            break;
        }
        else
        {
            data_list.pop_front();
        }
    }
    if(data_list.size() > 3)
    {
        if(data_list.at(1) != 0x62)
        {
            data_list.pop_front();
            return 0;
        }
    }
    else
    {
        return 2;
    }


    test_int = ubx_checksum(data_list);
    if(test_int == 1)
    {
        data_list.pop_front();
        data_list.pop_front();
        test_int = parse_ubx_gps(data_list);
        return (test_int);
    }
    if(test_int == 0)
    {
        data_list.pop_front();//THIS REMOVES THE FRONT BIT ON BAD CHECKSUM SO THE DATA CAN CONTINUE TO BE PARSED.
        return 0;
    }
    if(test_int == 2)
    {

        return 2;//not long enough please try again !
    }

    return 0;
}



int CERGO_GPS::ubx_checksum(std::deque <uint8_t> & data_list)
{
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;
    uint8_t UBX_length_hi = 0;
    auto data_iterator = data_list.begin();//sets the data iterator to the beginging of the list
    if(data_iterator != data_list.end())// moves one forward
    {
        data_iterator++;
    }
    else
    {
        return 2;
    }

    if(data_iterator != data_list.end())// moves one forward
    {
        data_iterator++;
    }
    else
    {
        return 2;
    }

    if(data_iterator != data_list.end())// moves one forward
    {
        data_iterator++;
    }
    else
    {
        return 2;
    }

    if(data_iterator != data_list.end())//moves one forward
    {
        data_iterator++;
    }
    else
    {
        return 2;
    }

    if(data_iterator != data_list.end())
    {
        UBX_length_hi = *data_iterator;// grabs the length
    }
    else
    {
        return 2;
    }

    data_iterator = data_list.begin();//resets the iterator
    data_iterator += 2;
    for(uint8_t i = 0 ; i <  (UBX_length_hi + 4) ; i++) //preforms the checksum!
    {
        if(data_iterator != data_list.end())
        {
            ck_a+=*data_iterator;
            ck_b+=ck_a;
            data_iterator++;
        }
        else
        {
            return 2;
        }
    }

    if(ck_a == *data_iterator)//checks the checksum
    {
        if(data_iterator != data_list.end())
        {
            data_iterator++;
        }
        else
        {
            return 2;
        }
        if(ck_b == *data_iterator)
        {
            return true;
        }
    }
    return false;
}

///******************************************************************///
///PACKETIZING-FUCTIONS

std::string CERGO_GPS::packatize()
{
    std::string time;
    std::string date;
    std::string lat;
    std::string lon;
    std::string alt;
    std::string data_string;
    long altitude = Altitude/10;
    long latitude = (Lattitude*3.6)/10;
    long longitude = (Longitude*3.6)/10;
    long nanosecondsD = towSubMsR + (1000000*(towMsR%1000));
    long nanosecondsD2 = towSubMsF + (1000000*(towMsF%1000));

    time = GPSTOWtoUTC(towMsR/1000);

    if(DEBUG_LEVEL >= 2 )
    {
        Log->add("Lat : %d Long : %d Alt : %d",latitude,longitude,altitude );
        Log->add("Fix = %d numsats= %d",gps_fix,gps_sat_numbers);
    }

    if(Lattitude == 0 || Longitude == 0)
    {
        int length = 0;
        char c = '\0';
        std::string value;
        std::ifstream file ( "/etc/ERGO/ERGO_DATA.csv" );
        length = file.tellg();//Get file size
        // loop backward over the file
        for(int i = length-2; i > 0; i-- )
        {
            file.seekg(i);
            c = file.get();
            if( c == '\r' || c == '\n' )//new line?
            {
                for ( int x = 0; x < 7; x++)
                {
                    getline ( file, value, ' ' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
                    if(DEBUG_LEVEL >=2)
                    {
                        Log->add("Value = %s \n",value.c_str());
                    }
                    if(i == 0)
                    {
                        date = value;
                    }
                    if(i == 3)
                    {
                        lat = value;
                    }
                    else if(i == 4)
                    {
                        lon = value;
                    }
                    else if(i == 5)
                    {
                        alt = value;
                    }
                }
            }
        }
    }
    else
    {
        date = date_packatize();
        lat = cordinate_packatize(latitude,100000000);
        lon = cordinate_packatize(longitude,100000000);
        alt = cordinate_packatize(altitude,1000000); //-0007736
    }
    std::string nsD = nanosecond_packatize(nanosecondsD);
    std::string nsD2 = nanosecond_packatize(nanosecondsD2);

    Log->data_add( date, time, unitid,  lat, lon , alt, nsD);
    return(data_string);

}

std::string CERGO_GPS::date_packatize()
{
    std::stringstream date_now;
    long day,month,year;

    long  Mjd = GpsToMjd( wnR, towMsR/1000);

    if(DEBUG_LEVEL >=3)
        Log->add("MJD = %ld",Mjd);

    MjdToDate(Mjd, year, month, day);


    if( month < 10 )
    {
        date_now << "0";
    }

    date_now << month << '/';

    if( day < 10 )
    {
        date_now << "0";
    }

    date_now <<  day << '/';

    year -= (2000 );
    if(year < 10)
    {
        date_now << "0";
    }

    date_now << year;

    return(date_now.str());
}

std::string CERGO_GPS::cordinate_packatize(long  cord, int length) // takes cord information and formats it into a string for packets
{
    std::stringstream cord_string;


    if (cord > 0)
    {
        cord_string << "+";
    }

    if (cord < 0)
    {
        cord_string << "-";
    }

    for (long i = 10; i <= length; i *= 10)
    {
        if (std::abs(cord) < i)
        {
            cord_string << "0";
        }
    }

    cord_string << (std::abs(cord));


    return (cord_string.str());
}

std::string CERGO_GPS::nanosecond_packatize(long  nS)
{
    std::stringstream nS_string;

    for (long i = 10; i <= 100000000; i *= 10)
    {
        if (std::abs(nS) < i)
        {
            nS_string << "0";
        }
    }
    nS_string << std::abs(nS);

    return (nS_string.str());
}

///******************************************************************///
///UBLOX FUCNTIONS
int CERGO_GPS::parse_ubx_gps(std::deque <uint8_t> & data_list)
{

    if(data_list.front() == 0x01)//ubx class
    {
        data_list.pop_front();//removes the class
        switch(data_list.front())//Checking the UBX ID
        {
        case 0x02: //ID NAV-POSLLH
            data_list.pop_front();//removes the ID
            data_list.pop_front();//removes length max
            data_list.pop_front();//removes length min
            if(gps_fix==0x03)
            {
                Time = join_4_bytes(data_list); // ms Time of week
                Longitude = join_4_bytes(data_list); // lon*10000000
                Lattitude = join_4_bytes(data_list); // lat*10000000
                Altitude = join_4_bytes(data_list);  // elipsoid heigth mm
            }
            else if(gps_fix==0x02)
            {
                Time = join_4_bytes(data_list); // ms Time of week
                Longitude = join_4_bytes(data_list); // lon*10000000
                Lattitude = join_4_bytes(data_list); // lat*10000000
            }

            data_list.pop_front();//pops checksum a
            data_list.pop_front();//pops checksum b
            return 4;
            break;
        case 0x03://ID NAV-STATUS
            break;
        case 0x06://ID NAV-SOL
            data_list.pop_front();//removes the ID
            data_list.pop_front();//removes length max
            data_list.pop_front();//removes length min
            if((data_list[10] >= 0x02)&&(data_list[11]&0x01))
            {
                gps_fix=data_list[10]; //valid position
                gps_sat_numbers=data_list[47];                    //Number of sats...
            }
            else
            {
                gps_fix=false; //invalid position
            }
            return 5;
            break;
        case 0x12:// ID NAV-VELNED
            break;
        }
        //  UBX_class = 0x0D;
    }
    else if(data_list.front()==0x0D)//checks the class
    {
        //UBX_id = 0x03;
        data_list.pop_front();
        switch(data_list.front())//Checking the UBX ID
        {
        case 0x03: //ID TIM-TM2
            data_list.pop_front();
            data_list.pop_front();
            data_list.pop_front();
            ch = one_byte(data_list);//marker channel 0 or 1
            flags = one_byte(data_list);//Bitmask
            counter = join_2_bytes(data_list);
            wnR = join_2_bytes(data_list);
            wnF = join_2_bytes(data_list);
            towMsR = join_4_bytes(data_list);
            towSubMsR = join_4_bytes(data_list);
            towMsF = join_4_bytes(data_list);
            towSubMsF = join_4_bytes(data_list);
            accEst = join_4_bytes(data_list);
            checksum = join_2_bytes(data_list);

            break;
        }
        return 3;
    }

    return 4;
}

///******************************************************************///
///DATE/TIME FUNCTIONS

long CERGO_GPS::DateToMjd (long current_year, long current_month, long current_day)
{
    return
        (367 * current_year
         - 7 * (current_year + (current_month + 9) / 12) / 4
         - 3 * ((current_year + (current_month - 9) / 7) / 100 + 1) / 4
         + 275 * current_month / 9
         + current_day
         + 1721028
         - 2400000);
}

void CERGO_GPS::MjdToDate (long &mjd, long &current_year, long &current_month, long &current_day)
{
    long J = 0;
    long CD = 0;
    long Y = 0;
    long M = 0;

    J = mjd + 2400001 + 68569;
    CD = 4 * J / 146097;
    J = J - (146097 * CD + 3) / 4;
    Y = 4000 * (J + 1) / 1461001;
    J = J - 1461 * Y / 4 + 31;
    M = 80 * J / 2447;
    current_day = J - 2447 * M / 80;
    J = M / 11;
    current_month = M + 2 - (12 * J);
    current_year = 100 * (CD - 49) + Y + J;
}

std::string CERGO_GPS::GPSTOWtoUTC(long secs)
{
    //converts the GPS Time of the Week presented by the ERGO GPS
// to the UTC "packed integer" format
    std::stringstream strstream;

//Useful constants
    long SEC_PER_DAY = 86400;
    long SEC_PER_HOUR = 3600;
    long SEC_PER_MINUTE = 60;
    long HOURS_PER_DAY = 24;
    long MINUTES_PER_DAY = 1440;
    long MINUTES_PER_HOUR = 60;
    long sleft;
    int offsetday = secs/SEC_PER_DAY;//today is tuesday
// 1514615313
//Serial.println(offsetday);
    sleft = secs - (offsetday * SEC_PER_DAY); //57300 secs left today
// Serial.println(sleft);
    int hs = sleft/SEC_PER_HOUR;//15 hours today
//Serial.println(hs);
    int sremainder;
    sremainder = sleft - (hs * SEC_PER_HOUR); // 3300 unused seconds... YET!
//Serial.println(sremainder);
    int mins = sremainder/SEC_PER_MINUTE; //55 and in this case, there are no seconds remaining
//Serial.println(mins);
    long stime = sremainder - (mins*SEC_PER_MINUTE); //This would be used if there were seconds left over
//Serial.println(stime);
    if (hs < 10)
    {
        strstream << "0";
    }
    strstream << hs;
    strstream << ":" ;
    if (mins < 10)
    {
        strstream << "0";
    }
    strstream << mins;
    strstream << ":";
    if (stime < 10)
    {
        strstream << "0";
    }
    strstream << stime;


    return strstream.str();
}

long CERGO_GPS::GpsToMjd( long gps_week, long gps_seconds)
{
    long gps_days;
    gps_days = ((0 * 1024) + gps_week) * 7 + (gps_seconds / 86400);
    return (DateToMjd(1980, 1, 6) + gps_days);
}

double diffclock( long clock1, long clock2 )
{

    double diffticks = clock1 - clock2;
    double diffms    = diffticks / ( CLOCKS_PER_SEC / 1000 );

    return diffms;
}

///******************************************************************/// BYTE-functions
/// Join 4 bytes into a long

long CERGO_GPS::join_4_bytes(std::deque<uint8_t> & data_list)
{
    union
    {
        long dword;
        uint8_t  byte[4];
    } long_union;
    memset(&long_union, 0, sizeof(long_union));

    for(int i = 0; i < 4; i++)
    {
        long_union.byte[i] = data_list.front();
        data_list.pop_front();
    }
    return(long_union.dword);
}

int CERGO_GPS::join_2_bytes(std::deque<uint8_t> & data_list)
{
    union
    {
        uint16_t word ;
        uint8_t  byte[2] ;
    } short_union;
    memset(&short_union, 0, sizeof(short_union));
    for(int i = 0; i < 2; i++)
    {
        short_union.byte[i] = data_list.front();
        data_list.pop_front();
    }

    return(short_union.word);
}

uint8_t CERGO_GPS::one_byte(std::deque<uint8_t> & data_list)
{
    union
    {
        uint8_t octet;
        uint8_t  byte[1];
    } octet_union;
    memset(&octet_union, 0, sizeof(octet_union));

    octet_union.byte[0] = data_list.front();
    data_list.pop_front();

    return(octet_union.octet);
}


///******************************************************************///

CERGO_GPS::~CERGO_GPS()
{
    //dtor
}

