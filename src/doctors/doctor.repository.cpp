#include "doctor.repository.hpp"

DoctorRepository::DoctorRepository(SqliteORM *orm) : orm(orm)
{
    this->doctors = this->orm->createEntity("doctors", {{"id", columnTypes::INT, true},
                                                        {"name", columnTypes::TEXT},
                                                        {"specialization", columnTypes::TEXT},
                                                        {"address", columnTypes::TEXT},
                                                        {"phone", columnTypes::TEXT}});
}

DoctorRepository::~DoctorRepository()
{
}

std::vector<std::map<std::string, std::string>> DoctorRepository::select(std::vector<std::string> conditions)
{
    return this->doctors->select(conditions);
}

bool DoctorRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->doctors->insert(values);
}

bool DoctorRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->doctors->update(id, values);
}

bool DoctorRepository::delete_(int id)
{
    return this->doctors->delete_(id);
}