#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace FileManager {

    inline bool WriteDataToFile(const std::string& filePath, const std::string& data) {
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            return false;
        }
        outFile << data;
        outFile.close();
        return true;
    }

    inline std::string ReadFromFile(const std::string& filePath) {
        std::ifstream inFile(filePath);
        if (!inFile.is_open()) {
            return "";
        }
        std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        return data;
    }
}