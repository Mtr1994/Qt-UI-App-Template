#include "appconfig.h"
#include "log/logger.h"

using namespace std;

AppConfig::AppConfig() : mConfigFile("conf.ini")
{

}

AppConfig *AppConfig::getInstance()
{
    static AppConfig inst;
    return &inst;
}

bool AppConfig::init()
{
    LOG_INFO("init config file");
    bool status = mConfigFile.read(mConfigData);
    return status;
}

std::string AppConfig::getValue(const std::string& section, const std::string& key)
{
    return mConfigData[section][key];
}

void AppConfig::setValue(const std::string &section, const std::string &key, const string &value)
{
    mConfigData[section][key] = value;
    mConfigFile.write(mConfigData);
}
