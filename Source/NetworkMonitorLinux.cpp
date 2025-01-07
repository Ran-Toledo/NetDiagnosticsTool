#include "NetworkMonitorLinux.h"
#include "CommandExecutor.h"
#include <string>
#include <fstream>

void NetworkMonitorLinux::collectStatistics(const std::string& scriptDirectory,
										    const std::string& outputDirectory,
										    const std::string& scriptArguments)
{
	CommandExecutor::executeCommand(scriptDirectory + "/monitor_network_stats.sh \"" + outputDirectory + "\" " + scriptArguments);
}

