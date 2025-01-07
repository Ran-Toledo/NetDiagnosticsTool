#include "StatisticsMonitorFactory.h"

// Platform-specific headers
#ifdef _WIN32
#include "SystemMonitorWindows.h"
#include "NetworkMonitorWindows.h"
#elif defined(__linux__)
#include "SystemMonitorLinux.h"
#include "NetworkMonitorLinux.h"
#endif

IStatisticsMonitor* StatisticsMonitorFactory::createSystemStatsMonitor()
{
    IStatisticsMonitor* monitor;

#ifdef _WIN32
    monitor = new SystemMonitorWindows();
#elif defined(__linux__)
    monitor = new SystemMonitorLinux();
#else
    monitor = nullptr; // Unsupported platform
#endif

    return monitor;
}

IStatisticsMonitor* StatisticsMonitorFactory::createNetworkStatsMonitor()
{
    IStatisticsMonitor* monitor;

#ifdef _WIN32
    monitor = new NetworkMonitorWindows();
#elif defined(__linux__)
    monitor = new NetworkMonitorLinux();
#else
    monitor = nullptr; // Unsupported platform
#endif

    return monitor;
}
