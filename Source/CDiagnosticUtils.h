// CDiagnosticUtils.h
#pragma once

#include <vector>
#include <string>

class CDiagnosticUtils
{
public:
    static std::vector<std::string> executeCommand(const std::string& command);
    static std::string getCurrentTimestamp();
    static std::string stripNonNumeric(const std::string& str);
};

