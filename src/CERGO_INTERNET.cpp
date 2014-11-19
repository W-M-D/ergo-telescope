#include "CERGO_INTERNET.h"

CERGO_INTERNET::CERGO_INTERNET(int debug_level)
{
    DEBUG_LEVEL = debug_level;
    MAX_INTERNET_TIMEOUT = 1000;
    first_pass = true;
    check_archive = true;
    internet_outage = true;
    internet_connection = false;
}

CERGO_INTERNET::CERGO_INTERNET()
{
    DEBUG_LEVEL = 0;
    MAX_INTERNET_TIMEOUT = 1000;
    first_pass = true;
    internet_outage = true;
    internet_connection = false;
}


bool CERGO_INTERNET::send_string(const std::string & data_string)
{
    CURL * curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string sending_string = "http://ergotelescope.org/postevent3.asp?dta=";
    sending_string.append(data_string);

    if(curl)
    {
        if(DEBUG_LEVEL >= 3)
        {
            Log->add("%s \n",sending_string.c_str());
        }
        curl_easy_setopt(curl,CURLOPT_NOSIGNAL ,1L );
        curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L );

        curl_easy_setopt(curl, CURLOPT_URL, sending_string.c_str());
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            return false;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        return true;
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
    return false;
}



void CERGO_INTERNET::manage_list()
{
    std::forward_list <std::string> string_list;
    string_list.emplace_front(" ");
    string_list.emplace_front(" ");
    while(true)
    {
        std::this_thread::sleep_for (std::chrono::seconds(1));
        if(check_archive)
        {
            mtx.lock();
            check_archive = Log->archive_load(string_list);
            while(!string_list.empty())
            {
                if(send_string(URLEncode(string_list.front().c_str()))) // calls the function that sends data to the server returns true on success
                {
                    if(DEBUG_LEVEL >= 1)
                    {
                        Log->add("Sent string : %s" , string_list.front().c_str());
                    }
                    if(internet_outage)
                    {
                        GPIO->setval_gpio(true,GPIO->YELLOW_MIDDLE);
                        internet_connection = true;
                        Log->add("CONNECTION RESTORED");
                        internet_outage = false;
                    }
                    string_list.pop_front();// pops the first element
                }
                else
                {
                    if(!internet_outage) // if the outage flag is not set
                    {
                        internet_connection = false;
                        internet_outage = true;

                        GPIO->setval_gpio(true,GPIO->YELLOW_MIDDLE);
                        Log->add("ERROR: COULD NOT SEND STRING");
                    }
                    break;
                }
            }
            mtx.unlock();

        }
    }
}

std::string CERGO_INTERNET::URLEncode(const char* msg)
{
    const char *hex = "0123456789abcdef";
    std::string encodedMsg = "";

    while (*msg!='\0')
    {
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') )
        {
            encodedMsg += *msg;
        }
        else
        {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}

CERGO_INTERNET::~CERGO_INTERNET()
{
    //dtor
}
