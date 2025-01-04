// CCommandLineParser.h
#pragma once

#include "CICommandLineParser.h"

#ifdef _WIN32
class CommandLineParserWindows : public CICommandLineParser {
public:
    void parseTypeperfOutput(const std::vector<std::string>& output, std::vector<std::string>& parsedOutput) override;
    void parsePingOutput(const std::vector<std::string>& output, double& latency, double& packetLoss) override;
    void parseNetStatOutput(const std::vector<std::string>& output, std::string& bytesReceived, std::string& bytesSent) override;
private:
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
};
#endif


#ifdef __linux__
class CommandLineParserLinux : public CICommandLineParser {
public:
    void parseTypeperfOutput(const std::vector<std::string>& output, std::vector<std::string>& parsedOutput) override;
    void parsePingOutput(const std::vector<std::string>& output, double& latency, double& packetLoss) override;
    void parseNetStatOutput(const std::vector<std::string>& output, std::string& bytesReceived, std::string& bytesSent) override;
};
#endif
