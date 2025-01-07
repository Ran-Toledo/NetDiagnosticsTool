#include "CommandExecutor.h"
#include <chrono>
#include <iomanip>

std::vector<std::string> CommandExecutor::executeCommand(const std::string& command)
{
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
