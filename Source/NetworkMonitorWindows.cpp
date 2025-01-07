#include "NetworkMonitorWindows.h"
#include "CommandExecutor.h"
#include <string>
#include <fstream>

NetworkMonitorWindows::NetworkMonitorWindows()
{
	// empty
}

void NetworkMonitorWindows::collectStatistics(const std::string& scriptDirectory,
											  const std::string& outputDirectory,
											  const std::string& scriptArguments)
{
	CommandExecutor::executeCommand(scriptDirectory + "/monitor_network_stats.bat \"" + outputDirectory + "\"");
}

