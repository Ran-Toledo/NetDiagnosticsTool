#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

class DatabaseManager {
public:
    // Constructor and destructor
    DatabaseManager(const std::string& csvFilePath, const std::string& dbFilePath);
    ~DatabaseManager();

    // Methods for reading CSV, importing to SQLite, and performing analysis
    void importCSVToDatabase(const std::string& tableName, const std::vector<std::string>& columnNames);
    void performBasicAnalysis();

private:
    std::string m_csvFilePath;
    std::string m_dbFilePath;
    sqlite3* m_db;

    // Helper methods for internal use
    std::vector<std::vector<std::string>> readCSV();
    void insertIntoDatabase(const std::string& tableName, const std::vector<std::string>& columnNames, const std::vector<std::vector<std::string>>& data);
    void analyzeData();
};