// CINetworkDiagnostics.h
#pragma once

#include <string>
#include <vector>

class CICommandLineParser;

class CINetworkDiagnostics
{
public:
    virtual ~CINetworkDiagnostics() {};
    virtual void monitorConnections() = 0;
    virtual void logNetworkDiagnostics() = 0;
    virtual void logSystemStatistics() = 0;

protected:
    static constexpr int s_DEFAULT_TIME_FRAME = 12U;
    static constexpr int s_DEFAULT_INTERVAL = 2U;

    CICommandLineParser* m_pParser;
    int m_interval;
    int m_timeFrame;
    std::string m_connectionAddress;

private:

};
