// INetworkStatsMonitor.h
#pragma once

#include <string>
#include "IStatisticsMonitor.h"

class INetworkStatsMonitor : public IStatisticsMonitor
{
public:
    virtual void collectStatistics(const std::string& scriptDirectory,
        const std::string& outputDirectory,
        const std::string& scriptArguments = "") = 0;

    virtual ~INetworkStatsMonitor() = default;

};
