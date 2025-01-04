#include "CCommandLineParser.h"
#include <sstream>

using namespace std;

#ifdef _WIN32
void CommandLineParserWindows::parseTypeperfOutput(const std::vector<std::string>& output, std::vector<std::string>& parsedOutput)
{
	for (auto& line : output) {
        if (line.find(",") != std::string::npos) {
	        const char delimiter = ',';
            stringstream ss(line);
            string item;

            while (std::getline(ss, item, delimiter)) {
                parsedOutput.push_back(item);
            }
        }
    }
}

void CommandLineParserWindows::parsePingOutput(const std::vector<std::string>& output,
										       double& latency,
										       double& packetLoss)
{
    for (const auto& line : output) {
        if (line.find("Average") != std::string::npos) {
            size_t start = line.find("Average = ") + 10;
            size_t end = line.find("ms", start);
            latency = std::stod(line.substr(start, end - start));
        }
        else if (line.find("Lost = ") != std::string::npos) {
            size_t start = line.find("Lost = ") + 7;
            size_t end = line.find("(", start);
            packetLoss = std::stod(line.substr(start, end - start));
        }
    }
}

void CommandLineParserWindows::parseNetStatOutput(const std::vector<std::string>& output,
    std::string& bytesReceived,
    std::string& bytesSent)
{
    for (int i = 4; i < output.size(); ++i)
    {
        string line = output[i];
        stringstream ss(line);
        string item;

        while (std::getline(ss, item))
        {
            if (item.find("Bytes") != std::string::npos)
            {
                // Split the line by spaces
                auto tokens = splitString(line, ' ');

                // Filter out empty tokens caused by multiple spaces
                std::vector<std::string> filteredTokens;
                for (const auto& token : tokens) {
                    if (!token.empty()) {
                        if (token.find('\n') != std::string::npos)
                        {
                            filteredTokens.push_back(token.substr(0, token.size() - 1));
                        }
                        else
                        {
                            filteredTokens.push_back(token);
                        }
                    }
                    
                }

                if (filteredTokens.size() >= 3) {
                    bytesReceived = filteredTokens[1];
                    bytesSent = filteredTokens[2];
                }
            }
        }
    }
}

std::vector<std::string> CommandLineParserWindows::splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> substrings;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        substrings.push_back(item);
    }

    return substrings;
}

#endif
