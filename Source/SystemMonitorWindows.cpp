#include "SystemMonitorWindows.h"
#include "CommandExecutor.h"
#include <stdexcept>

// Constructor
SystemMonitorWindows::SystemMonitorWindows()
{
	// empty
}

void SystemMonitorWindows::collectStatistics(const std::string& scriptDirectory,
	const std::string& outputDirectory,
	const std::string& scriptArguments)
{
	CommandExecutor::executeCommand(scriptDirectory + "/monitor_system_stats.bat \"" + outputDirectory + "\"");
}

