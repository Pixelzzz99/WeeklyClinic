#pragma once
#include "sqlite3.h"
#include "IEntity.hpp"

class SqliteEntity : public IEntity
{
    sqlite3 *db;
    std::string tableName;
    std::vector<column> columns;

public:
    SqliteEntity(sqlite3 *db, std::string tableName, std::vector<column> columns);
    ~SqliteEntity();

    bool insert(std::vector<std::pair<position, value>> values) override;

    bool select(std::string) override;

    bool update(std::string) override;

    bool delete_(std::string) override;

    bool drop() override;
};