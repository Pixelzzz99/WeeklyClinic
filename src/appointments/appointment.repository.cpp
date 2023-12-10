#include "SqliteORM.hpp"
#include "appointment.repository.hpp"

AppointmentRepository::AppointmentRepository(SqliteORM *orm) : orm(orm)
{
    this->appointments = orm->createEntity("appointment", {{"id", columnTypes::INT, true},
                                                           {"patient_id", columnTypes::TEXT, false, true, "patients", "patient_ID"},
                                                           {"doctor_id", columnTypes::INT, false, true, "doctors", "id"},
                                                           {"dateAppointment", columnTypes::TEXT},
                                                           {"timeAppointment", columnTypes::TEXT},
                                                           {"status", columnTypes::TEXT}});
}

AppointmentRepository::~AppointmentRepository()
{
}

std::vector<std::map<std::string, std::string>> AppointmentRepository::select(std::vector<std::string> conditions)
{
    return this->appointments->select(conditions);
}

bool AppointmentRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->appointments->insert(values);
}

bool AppointmentRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->appointments->update(id, values);
}

bool AppointmentRepository::delete_(int id)
{
    return this->appointments->delete_(id);
}
