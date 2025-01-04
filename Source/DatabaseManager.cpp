#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Constructor
DatabaseManager::DatabaseManager(const std::string& csvFilePath, const std::string& dbFilePath) :
	m_csvFilePath(csvFilePath),
	m_dbFilePath(dbFilePath),
	m_db(nullptr)
{
    int rc = sqlite3_open(m_dbFilePath.c_str(), &m_db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;
        throw std::runtime_error("Failed to open database");
    }
}

// Destructor
DatabaseManager::~DatabaseManager()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

// Read CSV file
std::vector<std::vector<std::string>> DatabaseManager::readCSV()
{
    std::ifstream file(m_csvFilePath);
    std::vector<std::vector<std::string>> data;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        throw std::runtime_error("Failed to open CSV file");
    }

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::istringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            row.push_back(token);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Insert data into SQLite database
void DatabaseManager::insertIntoDatabase(const std::string& tableName,
									     const std::vector<std::string>& columnNames, 
									     const std::vector<std::vector<std::string>>& data)
{
    // Create table if not exists
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + "id INTEGER PRIMARY KEY AUTOINCREMENT";
    for (const auto& column : columnNames) {
        createTableSQL += ", " + column + " TEXT";
    }
    createTableSQL += ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, createTableSQL.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        throw std::runtime_error("Failed to create table");
    }

    // Prepare SQL insert statement
    std::string insertSQL = "INSERT INTO " + tableName + " (";
    for (size_t i = 0; i < columnNames.size(); ++i) {
        insertSQL += columnNames[i];
        if (i < columnNames.size() - 1) {
            insertSQL += ", ";
        }
    }
    insertSQL += ") VALUES (";
    for (size_t i = 0; i < columnNames.size(); ++i) {
        insertSQL += "?";
        if (i < columnNames.size() - 1) {
            insertSQL += ", ";
        }
    }
    insertSQL += ");";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(m_db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        throw std::runtime_error("Failed to prepare insert statement");
    }

    for (const auto& row : data) {
        if (row.size() >= columnNames.size()) {
            for (size_t i = 0; i < columnNames.size(); ++i) {
                sqlite3_bind_text(stmt, static_cast<int>(i + 1), row[i].c_str(), -1, SQLITE_STATIC);
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Failed to execute statement: " << sqlite3_errmsg(m_db) << std::endl;
                sqlite3_reset(stmt);
                throw std::runtime_error("Failed to execute insert statement");
            }
            sqlite3_reset(stmt);
        }
    }

    sqlite3_finalize(stmt);
}

// Perform basic analysis on the data
void DatabaseManager::performBasicAnalysis() {
    std::string sql = "SELECT COUNT(*) FROM data;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        throw std::runtime_error("Failed to prepare select statement");
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        std::cout << "Total rows in data table: " << count << std::endl;
    }
    else {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

// Import CSV data to SQLite database
void DatabaseManager::importCSVToDatabase(const std::string& tableName, const std::vector<std::string>& columnNames) {
    auto data = readCSV();
    insertIntoDatabase(tableName, columnNames, data);
}
