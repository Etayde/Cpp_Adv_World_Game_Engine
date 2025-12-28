#include "DebugLog.h"

std::ofstream DebugLog::logFile;
bool DebugLog::initialized = false;

void DebugLog::init(const std::string& filename)
{
    if (!initialized)
    {
        logFile.open(filename, std::ios::out | std::ios::trunc);
        initialized = true;
    }
}

void DebugLog::close()
{
    if (initialized && logFile.is_open())
    {
        logFile.close();
        initialized = false;
    }
}

std::ofstream& DebugLog::getStream()
{
    if (!initialized)
    {
        init();
    }
    return logFile;
}
