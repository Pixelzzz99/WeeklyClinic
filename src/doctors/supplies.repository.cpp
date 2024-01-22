#include "supplies.repository.hpp"

SuppliesRepository::SuppliesRepository(SqliteORM *orm) : orm(orm)
{

    this->supplies = this->orm->createEntity("supplies", {{"id", columnTypes::INT, true},
                                                          {"name", columnTypes::TEXT},
                                                          {"quantity", columnTypes::INT},
                                                          {"price", columnTypes::FLOAT}});
}

SuppliesRepository::~SuppliesRepository()
{
}

std::vector<std::map<std::string, std::string>> SuppliesRepository::select(std::vector<std::string> conditions)
{
    return this->supplies->select(conditions);
}

bool SuppliesRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->supplies->insert(values);
}

bool SuppliesRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->supplies->update(id, values);
}

bool SuppliesRepository::delete_(int id)
{
    return this->supplies->delete_(id);
}