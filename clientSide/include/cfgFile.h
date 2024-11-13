#ifndef CFGFILE_H
#define CFGFILE_H

#include <string>
#include <unordered_map>

/**
 * @brief Parses a configuration file and stores the results in a map.
 * 
 * @param filename The name of the configuration file to parse.
 * @param configMap The map where configuration key-value pairs will be stored.
 * @return True if parsing was successful, false otherwise.
 */
bool parseConfigFile(const std::string &filename, std::unordered_map<std::string, std::string> &configMap);

/**
 * @brief Writes configuration data from a map to a file.
 * 
 * @param fileName The name of the file to write to.
 * @param config The map containing configuration key-value pairs.
 * @return True if writing was successful, false otherwise.
 */
bool writeConfigFile(const std::string &fileName, const std::unordered_map<std::string, std::string> &config);

#endif // CFGFILE_H