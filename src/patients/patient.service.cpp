#include "patient.service.hpp"

PatientService::PatientService(PatientRepository *repository) : repository(repository) {}

std::string PatientService::registerPatient()
{
}