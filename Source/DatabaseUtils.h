// DatabaseUtils.h
#pragma once

#include <string>
#include <vector>

class DatabaseUtils
{
public:
    static std::string escapeQuotes(const std::string& str);
    static std::string addQuotes(const std::string& str);
    static std::string parseRow(const std::string& line);
    static std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
    
};
