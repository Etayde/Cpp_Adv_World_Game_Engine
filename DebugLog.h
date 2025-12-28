#pragma once

#include <fstream>
#include <string>

class DebugLog
{
private:
    static std::ofstream logFile;
    static bool initialized;

public:
    static void init(const std::string& filename = "spring_debug.log");
    static void close();
    static std::ofstream& getStream();
};
