#include <iostream>
#include <string>
#include "SqliteORM.hpp"

SqliteEntity::SqliteEntity(sqlite3 *db, std::string tableName, std::vector<column> columns) : db(db), tableName(tableName), columns(columns)
{
    try
    {
        if (columns.empty())
        {
            throw std::runtime_error("Columns cannot be empty");
        }
        std::string createTable = "CREATE TABLE IF NOT EXISTS " + tableName + " (";

        for (const auto &column : columns)
        {
            createTable += column.name + " " + column.type;
            if (column.isPrimaryKey)
            {
                createTable += " PRIMARY KEY";
            }
            createTable += ", ";
        }

        createTable = createTable.substr(0, createTable.size() - 2);
        createTable += ")";
        sqlite3_exec(db, createTable.c_str(), 0, 0, 0);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

SqliteEntity::~SqliteEntity()
{
    std::string dropTable = "DROP TABLE " + this->tableName;
    char *errorMessage = nullptr;
    int result = sqlite3_exec(db, dropTable.c_str(), nullptr, nullptr, &errorMessage);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error dropping table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

bool SqliteEntity::insert(std::vector<std::pair<position, value>> values)
{
    try
    {
        if (values.empty())
        {
            throw std::runtime_error("Values cannot be empty");
        }
        std::string insertQuery = "INSERT INTO " + this->tableName + " (";
        std::string bindQuery = ") VALUES (";
        std::string valuesQuery = ")";

        for (auto column : this->columns)
        {
            if (column.isPrimaryKey)
            {
                continue;
            }
            insertQuery += column.name + ", ";
            bindQuery += "?, ";
        }

        insertQuery = insertQuery.substr(0, insertQuery.size() - 2);
        bindQuery = bindQuery.substr(0, bindQuery.size() - 2);

        std::string query = insertQuery + bindQuery + valuesQuery;

        sqlite3_stmt *stmt;

        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error("Internal server error");
        }

        for (auto value : values)
        {
            if (columns[value.first].type == columnTypes::TEXT || columns[value.first].type == columnTypes::CHAR)
            {
                std::string *text = new std::string(value.second);
                sqlite3_bind_text(stmt, value.first, text->c_str(), text->size(), SQLITE_STATIC);
            }
            else if (columns[value.first].type == columnTypes::INT)
            {
                sqlite3_bind_int(stmt, value.first, std::stoi(value.second));
            }
            else if (columns[value.first].type == columnTypes::FLOAT)
            {
                sqlite3_bind_double(stmt, value.first, std::stod(value.second));
            }
            else if (columns[value.first].type == columnTypes::BLOB)
            {
                sqlite3_bind_blob(stmt, value.first, value.second.c_str(), value.second.size(), SQLITE_STATIC);
            }
            else
            {
                throw std::runtime_error("Invalid column type");
            }
        }

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            std::cerr << "Failed to insert: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error("Internal server error");
        }

        sqlite3_finalize(stmt);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::vector<std::map<std::string, std::string>> SqliteEntity::select(std::vector<condition> conditions)
{
    try
    {
        std::string selectQuery = "SELECT * FROM " + this->tableName;
        sqlite3_stmt *stmt;

        if (conditions.empty())
        {
            int rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr);

            if (rc != SQLITE_OK)
            {
                std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("Internal server error");
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW)
            {
                std::cerr << "Failed to select: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("Internal server error");
            }

            std::vector<std::string> columnNames;
            std::vector<std::string> columnTypes;
            int columnCount = sqlite3_column_count(stmt);
            for (int i = 0; i < columnCount; i++)
            {
                const char *columnName = sqlite3_column_name(stmt, i);
                const char *columnType = sqlite3_column_decltype(stmt, i);
                columnNames.push_back(columnName);
                columnTypes.push_back(columnType ? columnType : "");
            }

            std::vector<std::map<std::string, std::string>> values;
            while (rc == SQLITE_ROW)
            {
                std::map<std::string, std::string> row;
                for (int i = 0; i < columnCount; i++)
                {
                    const char *value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
                    row[columnNames[i]] = value ? value : "";
                }
                values.push_back(row);
                rc = sqlite3_step(stmt);
            }

            sqlite3_finalize(stmt);

            return values;
        }

        selectQuery += " WHERE ";
        for (auto condition : conditions)
        {
            selectQuery += condition + " AND ";
        }
        selectQuery = selectQuery.substr(0, selectQuery.size() - 5);

        int rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error("Internal server error");
        }
        std::cout << "selectQuery: " << selectQuery << std::endl;
        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW)
        {
            return std::vector<std::map<std::string, std::string>>();
        }

        std::vector<std::string> columnNames;
        std::vector<std::string> columnTypes;
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; i++)
        {
            const char *columnName = sqlite3_column_name(stmt, i);
            const char *columnType = sqlite3_column_decltype(stmt, i);
            columnNames.push_back(columnName);
            columnTypes.push_back(columnType ? columnType : "");
        }

        std::vector<std::map<std::string, std::string>> values;
        while (rc == SQLITE_ROW)
        {
            std::map<std::string, std::string> row;
            for (int i = 0; i < columnCount; i++)
            {
                const char *value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
                row[columnNames[i]] = value ? value : "";
            }
            values.push_back(row);
            rc = sqlite3_step(stmt);
        }

        sqlite3_finalize(stmt);

        return values;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return {};
    }
}

bool SqliteEntity::update(int id, std::vector<std::pair<std::string, value>> values)
{
    if (this->tableName.empty())
    {
        std::cout << "table is not exist" << std::endl;
        return false;
    }
    std::string updateQuery = "UPDATE " + this->tableName + " SET ";
    try
    {
        if (values.empty())
        {
            return false;
        }
        for (const auto &value : values)
        {
            updateQuery += value.first + " = '" + value.second + "', ";
        }
        updateQuery = updateQuery.substr(0, updateQuery.size() - 2);
        updateQuery += " WHERE id = " + std::to_string(id);

        std::cout << updateQuery << std::endl;

        int rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to update: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error(sqlite3_errmsg(db));
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool SqliteEntity::delete_(int id)
{
    try
    {
        std::string deleteQuery = "DELETE FROM " + this->tableName + " WHERE id = " + std::to_string(id);
        int rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to delete: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error(sqlite3_errmsg(db));
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool SqliteEntity::drop()
{
    if (this->tableName == "")
    {
        std::cerr << "Table not exist" << std::endl;
        return false;
    }
    try
    {
        std::string dropQuery = "DROP TABLE " + this->tableName;
        int rc = sqlite3_exec(db, dropQuery.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to drop: " << sqlite3_errmsg(db) << std::endl;
            throw std::runtime_error(sqlite3_errmsg(db));
        }
        this->tableName = "";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}