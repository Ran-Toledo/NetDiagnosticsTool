#include "DatabaseUtils.h"
#include <sstream>

std::string DatabaseUtils::escapeQuotes(const std::string& str)
{
    std::string escaped;
    for (char ch : str)
    {
        if (ch == '\'')
        {
            escaped += "''"; // Escape single quote for SQLite
        }
        else {
            escaped += ch;
        }
    }
    return escaped;
}

std::string DatabaseUtils::addQuotes(const std::string& str)
{
    std::string quotedStr;

    if (str[0] != '\"') quotedStr += '\"';
    quotedStr += str;
    if (str[str.length() - 1] != '\"') quotedStr += '\"';

    return quotedStr;
}

std::string DatabaseUtils::parseRow(const std::string& line)
{
    std::istringstream lineStream(line);
    std::string cell, row;

    bool first = true;
    while (std::getline(lineStream, cell, ','))
    {
        if (!first) row += ", ";
        row += "'" + escapeQuotes(cell) + "'";
        first = false;
    }
    return row;
}

std::string DatabaseUtils::join(const std::vector<std::string>& elements, const std::string& delimiter)
{
    std::ostringstream os;

    for (size_t i = 0; i < elements.size(); ++i)
    {
        os << elements[i];
        if (i != elements.size() - 1)
        {
            os << delimiter;
        }
    }
    return os.str();
}