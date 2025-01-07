#include "DiagnosticsController.h"
#include <filesystem>
#include <algorithm>

int main(int argc, char* argv[])
{
	std::string targetIP;

	if (argc == 2) {
		targetIP = argv[1];
	}

	std::filesystem::path projectDir = std::filesystem::path(__FILE__).parent_path().parent_path();
	std::string projectDirStr = projectDir.string();
	std::replace(projectDirStr.begin(), projectDirStr.end(), '\\', '/');

	std::string dataDirectory = projectDirStr + "/Data";
	std::string scriptDirectory = projectDirStr + "/Scripts";

	// Sqlite Database file
	std::string dbFilePath = dataDirectory + "/NetDiagnostics.db";

	if (DiagnosticsController* diagnosticsController = DiagnosticsController::getInstance())
	{
		diagnosticsController->openDatabase(dbFilePath);
		diagnosticsController->setScriptDirectory(scriptDirectory);
		diagnosticsController->setOutputDirectory(dataDirectory);

		if (!targetIP.empty())
		{
			diagnosticsController->setTargetAddress(targetIP);
		}

		diagnosticsController->runDiagnostics();
	}

	return 0;
}
