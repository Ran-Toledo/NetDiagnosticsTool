#include "DiagnosticsController.h"
#include "StatisticsMonitorFactory.h"
#include <thread>
#include <iostream>

DiagnosticsController* DiagnosticsController::instance = nullptr;

DiagnosticsController::DiagnosticsController() :
    m_dbManager(DatabaseManager::getInstance())
{
    m_systemMonitor = StatisticsMonitorFactory::createSystemStatsMonitor();
    m_networkMonitor = StatisticsMonitorFactory::createNetworkStatsMonitor();
}

DiagnosticsController* DiagnosticsController::getInstance()
{
    if (!instance)
    {
        instance = new DiagnosticsController();
    }

    return instance;
}

DiagnosticsController::~DiagnosticsController()
{
    if (m_systemMonitor)
    {
        delete m_systemMonitor;
        m_systemMonitor = nullptr;
    }
    if (m_networkMonitor)
    {
        delete m_networkMonitor;
        m_networkMonitor = nullptr;
    }
    if (m_dbManager)
    {
        delete m_dbManager;
        m_dbManager = nullptr;
    }
}

void DiagnosticsController::collectSystemStats() const
{
    m_systemMonitor->collectStatistics(m_scriptDirectory, m_outputDirectory);
}

void DiagnosticsController::collectNetworkStats() const
{
    m_networkMonitor->collectStatistics(m_scriptDirectory, m_outputDirectory, m_targetAddress);
}

void DiagnosticsController::openDatabase(const std::string& dbFilePath) const
{
    if (m_dbManager)
    {
        m_dbManager->openDatabase(dbFilePath);
    }
}

void DiagnosticsController::setScriptDirectory(const std::string& scriptDirectory)
{
    m_scriptDirectory = scriptDirectory;
}

const std::string& DiagnosticsController::getScriptDirectory() const
{
    return m_scriptDirectory;
}

void DiagnosticsController::setOutputDirectory(const std::string& outputDirectory)
{
    m_outputDirectory = outputDirectory;
}

const std::string& DiagnosticsController::getOutputDirectory() const
{
    return m_outputDirectory;
}

void DiagnosticsController::setTargetAddress(const std::string& targetIP)
{
    m_targetAddress = targetIP;
}

const std::string& DiagnosticsController::getTargetAddress() const
{
    return m_targetAddress;
}


void DiagnosticsController::runDiagnostics()
{
    std::cout << "Starting all monitoring threads..." << std::endl;

    std::thread systemStatisticsThread(&DiagnosticsController::collectSystemStats, this);
    std::thread networkStatisticsThread(&DiagnosticsController::collectNetworkStats, this);

    systemStatisticsThread.join();
    std::cout << "System Statistics processing is complete." << std::endl;

    networkStatisticsThread.join();
    std::cout << "Network Statistics processing is complete." << std::endl;

    std::string csvPath = m_outputDirectory + "/system_stats.csv";
    m_dbManager->importCSVToDatabase(csvPath, "SystemStats");

    csvPath = m_outputDirectory + "/network_stats.csv";
    m_dbManager->importCSVToDatabase(csvPath, "NetworkStats");
}
