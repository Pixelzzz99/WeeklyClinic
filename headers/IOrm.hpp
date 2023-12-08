#pragma once
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include "IEntity.hpp"

class IOrm
{
public:
    virtual void connect(std::string dbName) = 0;
    virtual void disconnect() = 0;
    virtual std::shared_ptr<IEntity> createEntity(std::string tableName, std::vector<column> columns) = 0;
    virtual void drop() = 0;
};