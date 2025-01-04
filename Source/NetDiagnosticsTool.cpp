#include "NetworkDiagnostics.h"
#include "DatabaseManager.h"
#include <thread>


int main() {
    CINetworkDiagnostics* netDiagnostics;
#ifdef _WIN32
    netDiagnostics = new CNetworkDiagnosticsWindows("192.168.1.1");
#elif __linux__
    netDiagnostics = new CNetworkDiagnosticsLinux("192.168.1.1");
#endif
    std::thread monitor_thread([netDiagnostics] { netDiagnostics->monitorConnections(); });
    std::thread system_log_thread([netDiagnostics] { netDiagnostics->logSystemStatistics(); });
    std::thread network_log_thread([netDiagnostics] { netDiagnostics->logNetworkDiagnostics(); });
    
    monitor_thread.join();
    system_log_thread.join();
    network_log_thread.join();
    
    DatabaseManager dbManager("C:\\Users\\UGE\\Documents\\Projects\\NetDiagnosticsTool\\active_connections_log.csv", 
							   "C:\\Users\\UGE\\Documents\\Projects\\NetDiagnosticsTool\\Data\\netDiagnostics.db");
    std::vector<std::string> columnNames = { "Protocol", "Local_Address", "Foreign_Address", "State" };
    dbManager.importCSVToDatabase("ActiveConnections", columnNames);

    return 0;
}
