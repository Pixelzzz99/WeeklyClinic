#pragma once
#include "sqlite3.h"
#include "IEntity.hpp"

class SqliteEntity : public IEntity
{
    sqlite3 *db;
    std::string tableName;
    std::vector<column> columns;

    std::vector<std::string> getExistingColumns();
    std::vector<std::string> eraseExistingColumns(std::vector<std::string> new_columns, std::vector<std::string> existing_columns);
    bool alterTable();

public:
    SqliteEntity();
    SqliteEntity(sqlite3 *db, std::string tableName, std::vector<column> columns);
    ~SqliteEntity();

    bool insert(std::vector<std::pair<position, value>> values) override;

    std::vector<std::map<std::string, std::string>> select(std::vector<condition> conditions) override;

    bool update(int id, std::vector<std::pair<std::string, value>> values) override;

    bool delete_(int id) override;

    bool drop() override;
};