#include "DatabaseManager.h"
#include "DatabaseUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <map>

using namespace std;

DatabaseManager* DatabaseManager::instance = nullptr;

// Constructor
DatabaseManager::DatabaseManager() :
	m_db(nullptr),
	m_dbOpen(false)
{
	// empty
}

// Destructor
DatabaseManager::~DatabaseManager()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

DatabaseManager* DatabaseManager::getInstance()
{
	if (instance == nullptr)
	{
	    instance = new DatabaseManager();
	}

    return instance;
}

bool DatabaseManager::isDatabaseOpen() const
{
    return m_dbOpen;
}

void DatabaseManager::openDatabase(const std::string& dbFilePath)
{
    if (int rc = sqlite3_open(dbFilePath.c_str(), &m_db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;
        throw std::runtime_error("Failed to open database");
    }

    m_dbOpen = true;
    m_dbFilePath = dbFilePath;
}

void DatabaseManager::batchImportCSV(const std::vector<std::pair<std::string, std::string>>& csvFilesAndTables)
{
	for (const auto& [filePath, tableName] : csvFilesAndTables)
    {
        std::filesystem::path path(filePath);
        std::string fileName = path.filename().string();

		try {
			std::cout << "Importing " << fileName << " into table " << tableName << "..." << std::endl;
			importCSVToDatabase(filePath, tableName);
			std::cout << "Successfully imported " << fileName << " into table " << tableName << "." << std::endl;
	    }
	    catch (const std::exception& ex) {
			std::cerr << "Error importing " << fileName << " into table " << tableName << ": " << ex.what() << std::endl;
	    }
	}
}


void DatabaseManager::importCSVToDatabase(const std::string& csvFilePath, const std::string& tableName)
{
    std::ifstream file(csvFilePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file: " + csvFilePath);
    }

    std::string line, header;
    if (!std::getline(file, header)) 
    {
    	throw std::runtime_error("CSV file is empty: " + csvFilePath);
    }

    createTable(header, tableName);

    std::string sql = "INSERT INTO " + tableName + " VALUES ";
    std::vector<std::string> rows;

    while (std::getline(file, line))
    {
        rows.push_back("(" + DatabaseUtils::parseRow(line) + ")");
    }

    if (!rows.empty())
    {
        sql += DatabaseUtils::join(rows, ",") + ";";
        executeModifyQuery(sql);
    }
}

void DatabaseManager::executeSql(const std::string& sql)
{
    if (sql.substr(0, 6) == "SELECT" || sql.substr(0, 6) == "select") {
        try {
            auto results = executeSelectQuery(sql);

            for (const auto& row : results)
            {
                for (const auto& [column, value] : row)
                {
                    std::cout << value << " | ";
                }
                std::cout << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error executing SELECT query: " << e.what() << std::endl;
        }
    }
    else {
        try {
            executeModifyQuery(sql);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error executing modification query: " << e.what() << std::endl;
        }
    }
}

std::vector<std::string> DatabaseManager::getColumnNames(const std::string& tableName) const
{
	std::vector<std::string> columnNames;
	std::string query = "PRAGMA table_info(" + tableName + ");";

	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
	    throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(m_db)));
	}

	while (sqlite3_step(stmt) == SQLITE_ROW)
    {
	    const char* columnName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

	    if (columnName)
        {
	        columnNames.push_back(columnName);
	    }
	}

	sqlite3_finalize(stmt);
	return columnNames;
}

void DatabaseManager::createTable(const std::string& header, const std::string& tableName)
{
	std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
	std::istringstream headerStream(header);
	std::string column;

	bool first = true;
	while (std::getline(headerStream, column, ',')) {
        if (!first) sql += ", ";

        if (!column.empty() && column.front() != '\"')
        {
            sql += "\"";
        }

        sql += column;

        if (!column.empty() && column.back() != '\"')
        {
            sql += "\"";
        }

        sql += " TEXT";

        first = false;
    }
	sql += ");";

	executeModifyQuery(sql);
}

void DatabaseManager::executeModifyQuery(const std::string& query)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);
    char* errMsg = nullptr;

    if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = "SQLite error: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

std::vector<std::map<std::string, std::string>> DatabaseManager::executeSelectQuery(const std::string& query)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);
    sqlite3_stmt* stmt;

    int returnCode = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    if (returnCode != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare SQL statement: " + std::string(sqlite3_errmsg(m_db)));
    }

    std::vector<std::map<std::string, std::string>> result;
    int columnCount = sqlite3_column_count(stmt);

    while ((returnCode = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::map<std::string, std::string> row;
        for (int i = 0; i < columnCount; ++i)
        {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row[columnName] = columnValue ? columnValue : "NULL";
        }
        result.push_back(row);
    }

    if (returnCode != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error executing SQL query: " + std::string(sqlite3_errmsg(m_db)));
    }

    sqlite3_finalize(stmt);
    return result;
}


