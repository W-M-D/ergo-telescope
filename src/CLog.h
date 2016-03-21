#ifndef CLOG_H
#define CLOG_H
#include <ctime>
#include <mutex>          // std::mutex
#include <string.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <deque>
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <forward_list>
class CLog
{
public:
    CLog();

    void data_add(std::string &, std::string &, std::string &, std::string & ,std::string &,std::string &,std::string &);  //date, time, unit_id,  lat, on , alt, nanoseconds
    void archive_save(std::forward_list <std::string> &  );
    bool archive_load(std::forward_list <std::string> &  );
    void raw_hex_add(std::deque <uint8_t> &);

    std::streamoff last_sent_line_get();
    void reset_last_offset();
    void last_sent_line_save(std::streamoff ls);

    void add(const char * text , ...);
    std::string get_time();

    bool is_empty(std::ifstream& pFile);
    virtual ~CLog();

protected:
private:
    CLog * Log;
    std::streamoff last_offset;

};

#endif // CLOG_H
