#include "cfgFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <map>

bool parseConfigFile(const std::string &filename, std::unordered_map<std::string, std::string> &configMap) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open configuration file: " << filename << std::endl;
        return false;
    }

    std::string line, current_section;
    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); // Remove whitespace
        if (line.empty() || line[0] == ';') continue; // Skip empty lines or comments
        
        if (line[0] == '[' && line.back() == ']') {
            // New section
            current_section = line.substr(1, line.size() - 2);
        } else {
            auto delimiter_pos = line.find('=');
            if (delimiter_pos == std::string::npos) continue;

            std::string key = current_section + "." + line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);

            configMap[key] = value;
        }
    }

    file.close();
    return true;
}

bool writeConfigFile(const std::string &fileName, const std::unordered_map<std::string, std::string> &config) {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for saving game state: " << fileName << std::endl;
        return false;
    }

    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();  // Close the input file after reading into memory

    // create std::map to maintain sorted order
    std::map<std::string, std::string> sortedConfig(config.begin(), config.end());

    // Find position of [GameState] section
    size_t gameStatePos = fileContent.find("[GameState]");
    if (gameStatePos != std::string::npos) {
        // [GameState] section found, let's replace it
        size_t sectionStart = fileContent.find('\n', gameStatePos) + 1;  // Skip the header
        size_t sectionEnd = fileContent.find("[", sectionStart); // Find the next section header (if any)
        if (sectionEnd == std::string::npos) {
            sectionEnd = fileContent.length();  // If no next section, replace till the end of the file
        }

        // Remove the existing [GameState] section from the file content
        fileContent.erase(sectionStart, sectionEnd - sectionStart);

        // Insert new [GameState] values
        std::ostringstream gameStateStream;

        for (const auto& entry : sortedConfig) {
            gameStateStream << entry.first.substr(entry.first.find('.') + 1) << "=" << entry.second << "\n";
        } 

        // Insert the modified game state back into the file content
        fileContent.insert(sectionStart, gameStateStream.str());

    } else {
        // If no [GameState] section found, add it at the end
        std::ostringstream newGameStateStream;
        newGameStateStream << "[GameState]\n";
        for (const auto& entry : sortedConfig) {
            newGameStateStream << entry.first.substr(entry.first.find('.') + 1) << "=" << entry.second << "\n";
        }
        fileContent += newGameStateStream.str();
    }

    // Write the modified content back to the file
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing game state: " << fileName << std::endl;
        return false;
    }

    outFile << fileContent;
    outFile.close();  // Close the output file after writing

    return true;
}