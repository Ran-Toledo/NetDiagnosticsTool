#include "DiagnosticsController.h"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>

static std::string trim(const std::string& str)
{
	const auto first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
	{
		return "";
	}
	const auto last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, last - first + 1);
}

// Reads a simple "key = value" configuration file. Lines starting with '#' are ignored.
static std::map<std::string, std::string> loadConfig(const std::filesystem::path& configFilePath)
{
	std::map<std::string, std::string> config;
	std::ifstream file(configFilePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open configuration file: " + configFilePath.string());
	}

	std::string line;
	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		const auto separator = line.find('=');
		if (separator == std::string::npos)
		{
			continue;
		}

		config[trim(line.substr(0, separator))] = trim(line.substr(separator + 1));
	}

	return config;
}

// Returns the path stored under 'key', resolving relative paths against 'baseDir'.
static std::filesystem::path getConfigPath(const std::map<std::string, std::string>& config,
	const std::string& key, const std::filesystem::path& baseDir)
{
	const auto it = config.find(key);
	if (it == config.end() || it->second.empty())
	{
		throw std::runtime_error("Missing '" + key + "' in configuration file");
	}

	std::filesystem::path path(it->second);
	if (path.is_relative())
	{
		path = baseDir / path;
	}
	return path.lexically_normal();
}

static std::string toGenericString(const std::filesystem::path& path)
{
	std::string str = path.string();
	std::replace(str.begin(), str.end(), '\\', '/');
	return str;
}

int main(int argc, char* argv[])
{
	std::string targetIP;

	if (argc == 2) {
		targetIP = argv[1];
	}

	std::filesystem::path projectDir = std::filesystem::path(__FILE__).parent_path().parent_path();
	std::string projectDirStr = projectDir.string();
	std::replace(projectDirStr.begin(), projectDirStr.end(), '\\', '/');

	std::string scriptDirectory = projectDirStr + "/Scripts";

	// CSV log directory and Sqlite Database file are read from the configuration file
	std::string csvLogDirectory;
	std::string dbFilePath;
	try
	{
		const auto config = loadConfig(projectDir / "NetDiagnosticsTool.cfg");

		const auto csvLogPath = getConfigPath(config, "csv_log_directory", projectDir);
		std::filesystem::create_directories(csvLogPath);
		csvLogDirectory = toGenericString(csvLogPath);

		const auto dbPath = getConfigPath(config, "database_file", projectDir);
		if (dbPath.has_parent_path())
		{
			std::filesystem::create_directories(dbPath.parent_path());
		}
		dbFilePath = toGenericString(dbPath);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Configuration error: " << e.what() << std::endl;
		return 1;
	}

	if (DiagnosticsController* diagnosticsController = DiagnosticsController::getInstance())
	{
		diagnosticsController->openDatabase(dbFilePath);
		diagnosticsController->setScriptDirectory(scriptDirectory);
		diagnosticsController->setOutputDirectory(csvLogDirectory);

		if (!targetIP.empty())
		{
			diagnosticsController->setTargetAddress(targetIP);
		}

		diagnosticsController->runDiagnostics();
	}

	return 0;
}
