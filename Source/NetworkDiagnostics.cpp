#include "NetworkDiagnostics.h"
#include "CommandLineParser.h"
#include "DiagnosticUtils.h"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;

#ifdef _WIN32
CNetworkDiagnosticsWindows::CNetworkDiagnosticsWindows(const string& connection, int timeFrame, int interval) :
	CINetworkDiagnostics()
{
    m_timeFrame = timeFrame;
    m_interval = interval;
    m_connectionAddress = connection;
    m_pParser = new CommandLineParserWindows();
}

CNetworkDiagnosticsWindows::~CNetworkDiagnosticsWindows()
{
    if (m_pParser)
    {
        delete m_pParser;
        m_pParser = nullptr;
    }
}

void CNetworkDiagnosticsWindows::monitorConnections()
{
    cout << "Monitor Connections Thread started at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";


    std::ofstream logFile("active_connections_log.csv");
    logFile << "Protocol,Local Address,Foreign Address,State\n";
    //cout << "PROTOCOL\t\tLOCAL ADDRESS\t\t\tFOREIGN ADDRESS\t\t\tSTATE\n";

    // Get current timestamp
    string timestamp = CDiagnosticUtils::getCurrentTimestamp();

    vector<string> connections = CDiagnosticUtils::executeCommand("netstat -an");

    for (int j = 4; j < connections.size(); ++j)
    {
        string line = connections[j];
        stringstream ss(line);
        string item;

        while (ss >> item) {
            logFile << item << ",";
            //cout << item << "\t\t\t";
        }

        logFile << "\n";
        //cout << "\n";
    }

    cout << "Monitor Connections Thread ended at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";
}

void CNetworkDiagnosticsWindows::logNetworkDiagnostics()
{
    cout << "Network Diagnostics Log Thread started at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";

    std::ofstream logFile("network_diagnostics_log.csv");
    logFile << "Timestamp,Ping_Latency(ms),Packet_Loss(%),Bytes Received,Bytes Sent,Bandwidth_Usage(Bytes/sec)\n";

    vector<long> totalReceived;
    vector<long> totalSent;

    long long int previousNumOfBytesReceived = 0U;
    long long int previousNumOfBytesSent = 0U;

    for (int i = 0; i < m_timeFrame; ++i) {
        // Get current timestamp
        string timestamp = CDiagnosticUtils::getCurrentTimestamp();

        // Run and parse ping command
        double latency = 0.0, packetLoss = 0.0;
        auto pingOutput = CDiagnosticUtils::executeCommand("ping -n 4 " + m_connectionAddress);
        m_pParser->parsePingOutput(pingOutput, latency, packetLoss);

        // Calculate bandwith usage
        auto netstatOutput = CDiagnosticUtils::executeCommand("netstat -e");
        string bytesReceived, bytesSent;

        m_pParser->parseNetStatOutput(netstatOutput, bytesReceived, bytesSent);

        long long int numOfBytesReceived = std::stoll(bytesReceived);
        long long int numOfBytesSent = std::stoll(bytesSent);
        double bandwidthUsage = 0.0;

        // Calculate bandwidth usage
        if (previousNumOfBytesReceived > 0U && previousNumOfBytesSent > 0U)
        {
            long long int receivedBytes = numOfBytesReceived - previousNumOfBytesReceived;
            long long int sentBytes = numOfBytesSent - previousNumOfBytesSent;
            double receivedBandwidth = receivedBytes / m_interval;
            double sentBandwidth = sentBytes / m_interval;
            bandwidthUsage = receivedBandwidth + sentBandwidth;
        }

        previousNumOfBytesReceived = numOfBytesReceived;
        previousNumOfBytesSent = numOfBytesSent;

        // Log to CSV file
        logFile << timestamp << ","
            << latency << ","
            << packetLoss << ","
            << bytesReceived << ","
            << bytesSent << ","
            << bandwidthUsage << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(m_interval));
    }

    logFile.close();

    cout << "Network Diagnostics Log Thread ended at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";
}

void CNetworkDiagnosticsWindows::logSystemStatistics()
{
    cout << "System Statistics Log Thread started at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";

    std::ofstream logFile("system_statistics_log.csv");
    logFile << "Timestamp,CPU_Usage(%),Available_Memory(Bytes),Committed_Bytes,Cache_Bytes,Pages/sec\n";

    for (int i = 0; i < m_timeFrame; ++i) {
        // Get current timestamp
        string timestamp = CDiagnosticUtils::getCurrentTimestamp();

        // Run and parse typeperf commands
        vector<string> cpuUsageOutput;
    	m_pParser->parseTypeperfOutput(CDiagnosticUtils::executeCommand("typeperf \"\\Processor(_Total)\\% Processor Time\" -sc 1"), cpuUsageOutput);
        string cpuUsage = CDiagnosticUtils::stripNonNumeric(cpuUsageOutput[3]);

        vector<string> availableMemoryOutput;
    	m_pParser->parseTypeperfOutput(CDiagnosticUtils::executeCommand("typeperf \"\\Memory\\Available Bytes\" -sc 1"), availableMemoryOutput);
        string availableMemory = CDiagnosticUtils::stripNonNumeric(availableMemoryOutput[3]);

        vector<string> committedBytesOutput;
    	m_pParser->parseTypeperfOutput(CDiagnosticUtils::executeCommand("typeperf \"\\Memory\\Committed Bytes\" -sc 1"), committedBytesOutput);
        string committedBytes = CDiagnosticUtils::stripNonNumeric(committedBytesOutput[3]);

        vector<string> cacheBytesOutput;
    	m_pParser->parseTypeperfOutput(CDiagnosticUtils::executeCommand("typeperf \"\\Memory\\Cache Bytes\" -sc 1"), cacheBytesOutput);
        string cacheBytes = CDiagnosticUtils::stripNonNumeric(cacheBytesOutput[3]);

        vector<string> pagesSecOutput;
    	m_pParser->parseTypeperfOutput(CDiagnosticUtils::executeCommand("typeperf \"\\Memory\\Pages/sec\" -sc 1"), pagesSecOutput);
        string pagesSec = CDiagnosticUtils::stripNonNumeric(pagesSecOutput[3]);

        // Log to CSV file
        logFile << timestamp << ","
            << cpuUsage << ","
            << availableMemory << ","
            << committedBytes << ","
            << cacheBytes << ","
            << pagesSec << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(m_interval));
    }

    logFile.close();

    cout << "System Statistics Log Thread ended at " << CDiagnosticUtils::getCurrentTimestamp() << "\n";
}
#endif

#ifdef __linux__
void CNetworkDiagnosticsLinux::monitorConnections()
{
	
}
#endif