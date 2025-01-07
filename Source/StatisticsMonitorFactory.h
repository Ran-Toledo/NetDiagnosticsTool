// StatisticsMonitorFactory.h
#pragma once

#include "IStatisticsMonitor.h"

class StatisticsMonitorFactory
{
public:
    static IStatisticsMonitor* createSystemStatsMonitor();
    static IStatisticsMonitor* createNetworkStatsMonitor();
};
