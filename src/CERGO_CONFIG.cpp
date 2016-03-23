#include "CERGO_CONFIG.h"

CERGO_CONFIG::CERGO_CONFIG()
{
 config_file_name = "ergo-telescope.cfg";
 DEBUG_LEVEL = 0;
 version_number = 0;
 
}

int CERGO_CONFIG::load_config_file()
{
  using namespace std;
  using namespace libconfig; 
  Config ergo_telescope_config;
  
  try
  {
    ergo_telescope_config.readFile(config_file_name.c_str());
  }
  catch(const FileIOException &fioex)
  {
    Log->add("No ergo-telescope config file found starting with default config");
    return -1;
  }
  catch(const ParseException &pex)
  {
    Log->add("Error parsing ergo-telescope.cfg at line %d",pex.getLine());
    return -2;
  }
  
  try
  {
    version_number = ergo_telescope_config.lookup("version_number"); 
  }
  catch(const SettingNotFoundException &nfex)
  {
    Log->add("No version number in config file");
  }
  
  try
  {
    GPS_config_file_name = ergo_telescope_config.lookup("GPS_config_file").c_str();
  }
  catch(const SettingNotFoundException &nfex)
  {
    Log->add("No GPS_config_file_name found in ergo-telescope.cfg");
  }
  
  try
  {
    DEBUG_LEVEL = ergo_telescope_config.lookup("DEBUG_LEVEL");
  }
  catch(const SettingNotFoundException &nfex)
  {
    Log->add("No debug level found in ergo-telescope.cfg");
  }
  
  return 1;
}

CERGO_CONFIG::~CERGO_CONFIG()
{

}
