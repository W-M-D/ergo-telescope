#include <forward_list>
#include <unistd.h>
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "CLog.h"
#include <mutex>          // std::mutex
#include <signal.h>     /* signal, raise, sig_atomic_t */
#include "CERGO_GPIO.h"
class CERGO_INTERNET
{
public:
    CERGO_INTERNET(int,std::string url);
    CERGO_INTERNET();

    bool send_string(const std::string & data_string);
    void manage_list();
    std::string URLEncode(const char*);
    void reset_internet(clock_t &, int );
    size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp);



    virtual ~CERGO_INTERNET();


    void set_check_archive(std::atomic_bool & CHECK_ARCHIVE_BOOL)
    {
        check_archive.store(CHECK_ARCHIVE_BOOL,std::memory_order_release);
    }
protected:
private:
    std::mutex mtx;           // mutex for critical section
    bool internet_outage;
    std::atomic<bool> internet_connection;
    std::atomic<bool> check_archive;
    int internet_timeout;
    bool first_pass;
    clock_t internet_timer;
    clock_t log_check; // last time it checked if the log was open.
    CLog * Log;
    CERGO_GPIO * GPIO;
    std::string url_str = "";
    std::string read_string = "";
    int MAX_INTERNET_TIMEOUT;
    int DEBUG_LEVEL;

};

