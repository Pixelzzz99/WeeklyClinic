#include <iostream>
#include "SqliteORM.hpp"
#include "SqliteEntity.hpp"

SqliteORM::SqliteORM()
{
}

SqliteORM::SqliteORM(std::string dbName)
{
    this->connect(dbName);
}

SqliteORM::~SqliteORM()
{
    this->disconnect();
}

void SqliteORM::connect(std::string dbName)
{
    int rc = sqlite3_open(dbName.c_str(), &this->db);
    if (rc)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(this->db) << std::endl;
    }
}

void SqliteORM::disconnect()
{
    sqlite3_close(this->db);
}

std::shared_ptr<IEntity> SqliteORM::createEntity(std::string tableName, std::vector<column> columns)
{
    auto newEntity = std::make_shared<SqliteEntity>(this->db, tableName, columns);
    this->entities.push_back(newEntity);
    return newEntity;
}

void SqliteORM::drop()
{
    for (auto entity : this->entities)
    {
        entity->drop();
    }
}
