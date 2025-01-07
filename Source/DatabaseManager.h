// DatabaseManager.h
#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <sqlite3.h>
#include <map>

class DatabaseManager
{
public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    ~DatabaseManager();

    static DatabaseManager* getInstance();

    bool isDatabaseOpen() const;
    void openDatabase(const std::string& dbFilePath);
    void batchImportCSV(const std::vector<std::pair<std::string, std::string>>& csvFilesAndTables);
    void importCSVToDatabase(const std::string& csvFilePath, const std::string& tableName);
    void executeSql(const std::string& sql);
    std::vector<std::string> getColumnNames(const std::string& tableName) const;

private:
    DatabaseManager();

    void createTable(const std::string& header, const std::string& tableName);
    void executeModifyQuery(const std::string& query);
    std::vector<std::map<std::string, std::string>> executeSelectQuery(const std::string& query);

    sqlite3* m_db;
    std::string m_dbFilePath;
    std::mutex m_dbMutex;
    bool m_dbOpen;

    static DatabaseManager* instance;
};
