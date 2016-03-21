#include "CLog.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <zlib.h>
/*
USAGE
create a instance of the class

CLog * Log;


Then to add

use regular expresions to add using format string specifiers
http://www.cplusplus.com/reference/cstdio/printf/
more information on format strings here...

www.cplusplus.com/reference/cstdio/printf/

Log->add(" your string here %d,%f,..."42,42.42 )

*/

CLog::CLog()
{
    system("mkdir -p /etc/ERGO/");
}

void CLog::data_add(std::string & date, std::string & time, std::string & unit_id, std::string & lat,std::string & lon,std::string & alt,std::string & nanoseconds)
{
    std::stringstream uncompressed_string;
    gzFile data_file =  gzopen("/etc/ERGO/ERGO_DATA.csv.gz", "a9h");
    uncompressed_string << date << ' ' << time << ' ' << unit_id << ' ' << lat << ' ' << lon << ' ' << alt << ' ' << nanoseconds << '\n';
    
   
    gzprintf(data_file ,"%s", uncompressed_string.str().c_str() );
    gzclose(data_file);
}


void CLog::raw_hex_add(std::deque <uint8_t> & data_list)
{

    std::deque <uint8_t> print_list;
    std::ofstream hex_file;
    std::stringstream data_stream;
    std::string temp_string = "";
    print_list = data_list;
    hex_file.open("/etc/ERGO/ERGO_HEX.log",std::ios_base::out | std::ios_base::app);
    data_stream.flags(std::ios::hex | std::ios::showbase);
    while(!print_list.empty())
    {
        data_stream << print_list.front() << " ";
        print_list.pop_front();
    }
    temp_string = data_stream.str();
    hex_file << temp_string;
    hex_file.close();
   
}

bool CLog::is_empty(std::ifstream& data_in)
{
    return data_in.peek() == std::ifstream::traits_type::eof();
}


void CLog::add(const char * text , ...)
{
    char log_buff[512] ;

    std::ofstream log_file;
    log_file.open ("/etc/ERGO/ERGO_LOG.log", std::ios_base::out | std::ios_base::app); //creates log

    va_list va_alist;  //formats log and places into buffer
    memset(log_buff, 0, sizeof(log_buff)/sizeof(char));
    va_start ( va_alist, text );
    vsnprintf( log_buff + strlen( log_buff ),511, text, va_alist );
    va_end( va_alist );

    std::string log_text (log_buff);
    log_file << get_time() << " " << log_text << std::endl; // prints log
    log_file.close();
}

std::string CLog::get_time()
{
    std::stringstream time_now;
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    time_now << (now->tm_year + 1900) << '-'
             << (now->tm_mon + 1) << '-'
             <<  now->tm_mday << ' '
             <<  now->tm_hour <<':'
             <<  now->tm_min  <<':'
             <<  now->tm_sec;

    return(time_now.str());
}

CLog::~CLog()
{
    //dtor
}
