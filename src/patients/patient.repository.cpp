#include "patient.repository.hpp"

PatientRepository::PatientRepository(SqliteORM *orm) : orm(orm)
{
    this->patients = this->orm->createEntity("patients", {{"id", columnTypes::INT, true},
                                                          {"name", columnTypes::TEXT},
                                                          {"age", columnTypes::INT},
                                                          {"gender", columnTypes::TEXT},
                                                          {"address", columnTypes::TEXT},
                                                          {"phone", columnTypes::TEXT},
                                                          {"medical_history", columnTypes::TEXT},
                                                          {"count_visits", columnTypes::INT},
                                                          {"patient_ID", columnTypes::TEXT}});
}

PatientRepository::~PatientRepository()
{
}

std::vector<std::map<std::string, std::string>> PatientRepository::select(std::vector<std::string> conditions)
{
    return this->patients->select(conditions);
}

bool PatientRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->patients->insert(values);
}

bool PatientRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->patients->update(id, values);
}

bool PatientRepository::delete_(int id)
{
    return this->patients->delete_(id);
}
