#pragma once
#include "IOrm.hpp"
#include "SqliteEntity.hpp"
#include "sqlite3.h"

class SqliteORM : public IOrm
{
    sqlite3 *db;
    std::vector<std::shared_ptr<IEntity>> entities;

public:
    SqliteORM();
    SqliteORM(std::string dbName);
    ~SqliteORM();

    void connect(std::string dbName) override;

    void disconnect() override;

    std::shared_ptr<IEntity> createEntity(std::string tableName, std::vector<column> columns) override;

    void drop() override;
};