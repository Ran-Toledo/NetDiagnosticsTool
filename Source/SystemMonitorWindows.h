// SystemMonitorWindows.h
#pragma once

#include "ISystemStatsMonitor.h"
#include <string>
#include <vector>

class SystemMonitorWindows : public ISystemStatsMonitor
{
public:
    enum class SystemStat
    {
        FirstCounter = 0U,
        CPUUsage = FirstCounter,
        MemoryAvailableBytes,
        MemoryCommittedBytes,
        MemoryCacheBytes,
        MemoryPagesSec,
        NumOfCounters
    };

    SystemMonitorWindows();

    void collectStatistics(const std::string& scriptDirectory, const std::string& outputDirectory, const std::string& scriptArguments) override;
    //static const std::vector<std::string>& getAllFiles();

private:
    //void collectSystemStat(SystemStat stat) const;
    //static std::string getCommand(SystemStat stat);
    //static std::string getFileName(SystemStat stat);
    //
    // std::string m_outputPath;
};

