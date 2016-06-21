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
  std::string CONFIG_FILE_PATH = "/etc/ERGO/";
  std::string full_config_file_name = CONFIG_FILE_PATH;
  full_config_file_name.append(GPS_config_file_name); 
  return full_config_file_name;
}
std::string get_config_file_name()
{
  std::string CONFIG_FILE_PATH = "/etc/ERGO/";
  std::string full_config_file_name = CONFIG_FILE_PATH;
  full_config_file_name.append(config_file_name); 
  return full_config_file_name;
}

int get_debug_level()
{
  return DEBUG_LEVEL;
  
}

std::string get_version_number()
{
  return version_number;
}

int load_config_file();

private:
    int DEBUG_LEVEL;
    std::string GPS_config_file_name;
    std::string config_file_name;
    std::string version_number;
    CLog * Log;
};

#endif // CERGO_CONFIG_H
