#ifndef _configuration_h
#define _configuration_h

#include <string>
#include <map>
#include <any>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class Configuration
{
private:
    std::map<std::string, std::any> config;
    std::map<std::string, std::any> pluginConfig;
    std::map<std::string, unsigned int> configArraySizes;
    bool loaded = false;

public:
    bool LoadConfiguration();
    inline bool IsConfigurationLoaded() { return this->loaded; };

    void LoadPluginConfigurations();

    std::map<std::string, std::any>& FetchPluginConfiguration() { return this->pluginConfig; }
    std::map<std::string, std::any>& FetchConfiguration() { return this->config; }
    std::map<std::string, unsigned int> FetchConfigArraySizes() { return this->configArraySizes; }

    void SetArraySize(std::string key, unsigned int size);

    template <typename T>
    T FetchValue(std::string key);

    template <typename T>
    void SetValue(std::string key, T value);

    template <typename T>
    void SetPluginValue(std::string key, T value);

    bool HasKey(std::string key) { return (this->config.find(key) != this->config.end()); }
    void LoadPluginConfig(std::string key);
    void ClearPluginConfig();
};

extern Configuration* g_Config;

template <typename T>
T Configuration::FetchValue(std::string key)
{
    if (this->config.find(key) == this->config.end())
        return 0;

    try {
        return std::any_cast<T>(this->config[key]);
    } catch(std::bad_any_cast& e) {
        fprintf(stdout, "%s: %s\n", key.c_str(), e.what());
        return (T)0;
    }
}

void WritePluginFile(std::string path, rapidjson::Value& val);

#endif