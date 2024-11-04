#ifndef CFGFILE_H
#define CFGFILE_H

#include <string>
#include <unordered_map>

bool parseConfigFile(const std::string &filename, std::unordered_map<std::string, std::string> &configMap);
bool writeConfigFile(const std::string &fileName, const std::unordered_map<std::string, std::string> &config);

#endif // CFGFILE_H