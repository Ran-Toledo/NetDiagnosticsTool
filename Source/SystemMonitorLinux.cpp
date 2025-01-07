#include "SystemMonitorLinux.h"
#include "CommandExecutor.h"
#include <stdexcept>

void SystemMonitorLinux::collectStatistics(const std::string& scriptDirectory,
										   const std::string& outputDirectory,
										   const std::string& scriptArguments)
{
	CommandExecutor::executeCommand(scriptDirectory + "/monitor_system_stats.sh \"" + outputDirectory + "\"");
}

