// IStatisticsMonitor.h
#pragma once

#include <string>

class IStatisticsMonitor
{
public:
    virtual void collectStatistics(const std::string& scriptDirectory,
        const std::string& outputDirectory,
        const std::string& scriptArguments = "") = 0;

    virtual ~IStatisticsMonitor() = default;

};
