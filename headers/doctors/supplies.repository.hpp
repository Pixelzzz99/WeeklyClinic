#pragma once
#include "SqliteORM.hpp"
#include "SqliteEntity.hpp"

class SuppliesRepository
{

    SqliteORM *orm;
    std::shared_ptr<IEntity> supplies;

public:
    SuppliesRepository(SqliteORM *orm);
    ~SuppliesRepository();

    std::vector<std::map<std::string, std::string>> select(std::vector<std::string> conditions);

    bool insert(std::vector<std::pair<int, std::string>> values);

    bool update(int id, std::vector<std::pair<std::string, std::string>> values);

    bool delete_(int id);
};