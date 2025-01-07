// NetworkMonitorWindows.h
#pragma once

#include "INetworkStatsMonitor.h"
#include <string>

class NetworkMonitorWindows : public INetworkStatsMonitor
{
public:
    enum class NetworkStat
    {
        FirstStat = 0U,
        BytesTotalPerSec = FirstStat,
        PacketsPerSec,
        PacketsReceivedPerSec,
        PacketsSentPerSec,
        TCPActiveConnections,
        TCPConnectionsEstablished,
        UDPDatagramsSent,
        NumOfStats
    };

    NetworkMonitorWindows();

    void collectStatistics(const std::string& scriptDirectory, const std::string& outputDirectory, const std::string& scriptArguments) override;
    // static const std::vector<std::string>& getAllFiles();

private:
    // void collectNetworkStat(NetworkStat stat) const;
    // static std::string getCommand(NetworkStat stat);
    // static std::string getFileName(NetworkStat stat);
    //
    // std::string m_outputPath;
};

