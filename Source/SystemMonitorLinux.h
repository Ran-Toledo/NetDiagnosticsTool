// SystemMonitorLinux.h
#pragma once

#include "ISystemStatsMonitor.h"
#include <string>

class SystemMonitorLinux : public ISystemStatsMonitor
{
public:
    SystemMonitorLinux() = default;

    void collectStatistics(const std::string& scriptDirectory,
        const std::string& outputDirectory,
        const std::string& scriptArguments) override;

};

