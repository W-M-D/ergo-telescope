#ifndef CERGO_CONFIG_H
#define CERGO_CONFIG_H
#include <libconfig.h++>
#include "CLog.h"


class CERGO_CONFIG
{
public:
CERGO_CONFIG();
~CERGO_CONFIG();
std::string get_GPS_config_file_name()
{
  std::string CONFIG_FILE_PATH = "/etc/ERGO";
  std::string full_config_file_name = config_file_name;
  full_config_file_name.append(CONFIG_FILE_PATH); 
  return full_config_file_name;
}

int get_debug_level()
{
  return DEBUG_LEVEL;
  
}

double get_version_number()
{
  return version_number;
}

int load_config_file();

private:
    int DEBUG_LEVEL;
    std::string GPS_config_file_name;
    static ::string config_file_name="/etc/ERGO/ergo-telescope.cfg";
    double version_number;
    CLog * Log;
};

#endif // CERGO_CONFIG_H
