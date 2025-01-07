// DiagnosticsController.h
#pragma once

#include "IStatisticsMonitor.h"
#include "DatabaseManager.h"

class DiagnosticsController
{
public:
    DiagnosticsController(const DiagnosticsController&) = delete;
    DiagnosticsController& operator=(const DiagnosticsController&) = delete;
    ~DiagnosticsController();

    static DiagnosticsController* getInstance();

    void runDiagnostics();
    void openDatabase(const std::string& dbFilePath) const;
    void setScriptDirectory(const std::string& scriptDirectory);
    const std::string& getScriptDirectory() const;
    void setOutputDirectory(const std::string& outputDirectory);
    const std::string& getOutputDirectory() const;
    void setTargetAddress(const std::string& targetIP);
    const std::string& getTargetAddress() const;

private:
    DiagnosticsController();

    void collectSystemStats() const;
    void collectNetworkStats() const;

    static DiagnosticsController* instance;

    IStatisticsMonitor* m_systemMonitor;
    IStatisticsMonitor* m_networkMonitor;
    DatabaseManager* m_dbManager;
    std::string m_scriptDirectory;
    std::string m_outputDirectory;
    std::string m_targetAddress;

};
