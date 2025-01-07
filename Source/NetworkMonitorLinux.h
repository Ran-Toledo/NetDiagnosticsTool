// NetworkMonitorLinux.h
#pragma once

#include "INetworkStatsMonitor.h"
#include <string>

class NetworkMonitorLinux : public INetworkStatsMonitor
{
public:
    NetworkMonitorLinux() = default;

    void collectStatistics(const std::string& scriptDirectory,
        const std::string& outputDirectory,
        const std::string& scriptArguments = "") override;

};

