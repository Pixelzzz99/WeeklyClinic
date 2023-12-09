#pragma once
#include "SqliteORM.hpp"
#include "SqliteEntity.hpp"

class DoctorRepository
{
    SqliteORM *orm;
    std::shared_ptr<IEntity> doctors;

public:
    DoctorRepository(SqliteORM *orm);
    ~DoctorRepository();

    std::vector<std::map<std::string, std::string>> select(std::vector<std::string> conditions);

    bool insert(std::vector<std::pair<int, std::string>> values);

    bool update(int id, std::vector<std::pair<std::string, std::string>> values);

    bool delete_(int id);
};