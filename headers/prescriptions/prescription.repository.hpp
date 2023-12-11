#pragma once
#include "SqliteORM.hpp"
#include "SqliteEntity.hpp"

class PrescriptionRepository
{

    SqliteORM *orm;
    std::shared_ptr<IEntity> prescriptions;

public:
    PrescriptionRepository(SqliteORM *orm);
    ~PrescriptionRepository();

    std::vector<std::map<std::string, std::string>> select(std::vector<std::string> conditions);

    bool insert(std::vector<std::pair<int, std::string>> values);

    bool update(int id, std::vector<std::pair<std::string, std::string>> values);

    bool delete_(int id);
};
