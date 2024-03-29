#include "CERGO_INTERNET.h"

CERGO_INTERNET::CERGO_INTERNET(int debug_level,std::string url)
{
    if(!url.empty())
    {
        this->url_str = "http://ergotelescope.org/postevent3.asp?dta=";
    }
    else
    {
        this->url_str = url;
    }
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


size_t CERGO_INTERNET::write_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool CERGO_INTERNET::send_string(const std::string & data_string)
{
    CURL * curl;
    CURLcode res;
    this->read_string = "";
    curl = curl_easy_init();
    std::string sending_string = this->url_str;
    sending_string.append(data_string);

    if(curl)
    {
        if(DEBUG_LEVEL >= 5)
        {
            Log->add("%s \n",sending_string.c_str());
        }
        curl_easy_setopt(curl,CURLOPT_NOSIGNAL ,1L );
        curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L );
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&this->read_string);

        curl_easy_setopt(curl, CURLOPT_URL, sending_string.c_str());
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            return false;
        }
        if(this->read_string.find("error") != std::string::npos )
        {
            if(DEBUG_LEVEL >= 1)
            {
                Log->add("%s \n",read_string.c_str());
            }
            return(false);
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

            mtx.lock();
            check_archive = Log->archive_load(string_list);
            while(!string_list.empty())
            {
                if(send_string(URLEncode(string_list.front().c_str()))) // calls the function that sends data to the server returns true on success
                {
                    if(DEBUG_LEVEL >= 1)
                    {
                        Log->add("Sent string : %s\n" , string_list.front().c_str());
                    }
                    if(internet_outage)
                    {
                        internet_connection = true;
                        internet_outage = false;
                        GPIO->setval_gpio(internet_connection,GPIO->YELLOW_MIDDLE);
                        Log->add("CONNECTION RESTORED");
                    }
                    string_list.pop_front();// pops the first element
                }
                else
                {
                    if(!internet_outage) // if the outage flag is not set
                    {
                        internet_connection = false;
                        internet_outage = true;
                        GPIO->setval_gpio(internet_connection,GPIO->YELLOW_MIDDLE);
                        Log->add("ERROR: COULD NOT SEND STRING");
                    }
                    break;
                }
            }
            mtx.unlock();


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
