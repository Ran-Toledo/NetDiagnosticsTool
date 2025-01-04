// CNetworkDiagnosticsWindows.h
#pragma once

#include "CINetworkDiagnostics.h"

#ifdef _WIN32
class CNetworkDiagnosticsWindows : public CINetworkDiagnostics
{
public:
    CNetworkDiagnosticsWindows(const std::string& connection, int timeFrame = s_DEFAULT_TIME_FRAME, int interval = s_DEFAULT_INTERVAL);
    ~CNetworkDiagnosticsWindows() override;
    void monitorConnections() override;
    void logNetworkDiagnostics() override;
    void logSystemStatistics() override;
private:

};
#endif

#ifdef __linux__
class CNetworkDiagnosticsLinux : public CINetworkDiagnostics
{
public:
    void monitorConnections() override;
    //void logNetworkDiagnostics() override;
    //void logSystemStatistics() override;
private:
    //static std::vector<std::string> executeCommand(const std::string& command);
    //static std::string getCurrentTimestamp();
    //static std::string stripNonNumeric(const std::string& str);
};
#endif

