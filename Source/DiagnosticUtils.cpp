#include "DiagnosticUtils.h"
#include <chrono>
#include <sstream>
#include <iomanip>
//#include <unistd.h>

std::vector<std::string> CDiagnosticUtils::executeCommand(const std::string& command) {
    std::vector<std::string> result;
    std::string line;
    FILE* pipe;
#ifdef _WIN32
    pipe = _popen(command.c_str(), "r");
#elif __linux__
    pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result.push_back(buffer);
        }
    }
    catch (...) {
#ifdef _WIN32
        _pclose(pipe);
#elif __linux__
        pclose(pipe);
#endif
        throw;
    }
#ifdef _WIN32
    _pclose(pipe);
#elif __linux__
    pclose(pipe);
#endif
    return result;
}

std::string CDiagnosticUtils::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);

    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm now_tm;
#ifdef _WIN32
    localtime_s(&now_tm, &now_t);
#elif __linux__
    localtime_r(&now_t, &now_tm);
#endif

    std::ostringstream timestamp;
    timestamp << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << now_ms.count();

    return timestamp.str();
}

std::string CDiagnosticUtils::stripNonNumeric(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if ((ch >= '0' && ch <= '9') || ch == '.') {
            result += ch;
        }
    }
    return result;
}
