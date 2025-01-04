// CICommandLineParser.h
#pragma once

#include <vector>
#include <string>

class CICommandLineParser
{
public:
    virtual ~CICommandLineParser() {}
    virtual void parseTypeperfOutput(const std::vector<std::string>& output, std::vector<std::string>& parsedOutput) = 0;
    virtual void parsePingOutput(const std::vector<std::string>& output, double& latency, double& packetLoss) = 0;
    virtual void parseNetStatOutput(const std::vector<std::string>& output, std::string& bytesReceived, std::string& bytesSent) = 0;
};
