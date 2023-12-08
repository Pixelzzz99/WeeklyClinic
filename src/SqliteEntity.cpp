#include <iostream>
#include "SqliteORM.hpp"

SqliteEntity::SqliteEntity(sqlite3 *db, std::string tableName, std::vector<column> columns)
{
    try
    {
        if (columns.empty())
        {
            throw std::runtime_error("Columns cannot be empty");
        }
        this->db = db;
        this->tableName = tableName;
        std::string createTable = "CREATE TABLE IF NOT EXISTS " + tableName + " (";

        for (auto column : columns)
        {
            this->columns.push_back(column);
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
    sqlite3_exec(db, dropTable.c_str(), 0, 0, 0);
}

bool SqliteEntity::insert(std::vector<std::pair<position, value>> values)
{
    try
    {
        if (values.empty())
        {
            throw std::runtime_error("Values cannot be empty");
        }

        sqlite3_stmt *stmt;

        std::string insertQuery = "INSERT INTO " + this->tableName + " (";

        for (auto column : this->columns)
        {
            if (column.isPrimaryKey)
            {
                continue;
            }
            insertQuery += column.name;
            insertQuery += ", ";
        }

        insertQuery = insertQuery.substr(0, insertQuery.size() - 2);
        insertQuery += ") VALUES (";

        for (auto column : this->columns)
        {
            if (column.isPrimaryKey)
            {
                continue;
            }
            insertQuery += "?";
            insertQuery += ", ";
        }

        insertQuery = insertQuery.substr(0, insertQuery.size() - 2);
        insertQuery += ")";

        int rc = sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &stmt, 0);

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

IEntity::rows SqliteEntity::select(std::vector<condition> conditions)
{
    try
    {
        std::string selectQuery = "SELECT * FROM " + this->tableName;
        sqlite3_stmt *stmt;

        if (conditions.empty())
        {
            int rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, 0);

            if (rc != SQLITE_OK)
            {
                std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("Internal server error");
            }

            rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE)
            {
                std::cerr << "Failed to select: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("Internal server error");
            }

            rows allRows;

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                std::vector<std::string> oneRow = {};

                for (auto column : this->columns)
                {
                    if (column.isPrimaryKey)
                    {
                        continue;
                    }
                    if (column.type == columnTypes::TEXT || column.type == columnTypes::CHAR)
                    {
                        oneRow[column.name] = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, column.position)));
                    }
                    else if (column.type == columnTypes::INT)
                    {
                        oneRow[column.name] = std::to_string(sqlite3_column_int(stmt, column.position));
                    }
                    else if (column.type == columnTypes::FLOAT)
                    {
                        oneRow[column.name] = std::to_string(sqlite3_column_double(stmt, column.position));
                    }
                    else if (column.type == columnTypes::BLOB)
                    {
                        oneRow[column.name] = std::string(reinterpret_cast<const char *>(sqlite3_column_blob(stmt, column.position)), sqlite3_column_bytes(stmt, column.position));
                    }
                }
            }

            sqlite3_finalize(stmt);

            return true;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool SqliteEntity::update(std::string)
{

    return true;
}

bool SqliteEntity::delete_(std::string)
{

    return true;
}

bool SqliteEntity::drop()
{

    return true;
}