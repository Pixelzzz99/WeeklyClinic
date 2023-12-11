#include "prescription.repository.hpp"

PrescriptionRepository::PrescriptionRepository(SqliteORM *orm) : orm(orm)
{
    this->prescriptions = this->orm->createEntity("prescriptions", {
                                                                       {"id", columnTypes::INT, true},
                                                                       {"patient_id", columnTypes::TEXT, false, true, "patients", "patient_ID"},
                                                                       {"doctor_id", columnTypes::INT, false, true, "doctors", "id"},
                                                                       {"prescription", columnTypes::TEXT},
                                                                       {"date", columnTypes::TEXT},
                                                                   });
}

PrescriptionRepository::~PrescriptionRepository()
{
}

std::vector<std::map<std::string, std::string>> PrescriptionRepository::select(std::vector<std::string> conditions)
{
    return this->prescriptions->select(conditions);
}

bool PrescriptionRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->prescriptions->insert(values);
}

bool PrescriptionRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->prescriptions->update(id, values);
}

bool PrescriptionRepository::delete_(int id)
{
    return this->prescriptions->delete_(id);
}
