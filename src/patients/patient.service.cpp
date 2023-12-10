#include "patient.service.hpp"
#include <uuid/uuid.h>

PatientService::PatientService(std::shared_ptr<PatientRepository> repository) : repository(repository) {}

std::string PatientService::registerPatient(std::string name, int age, std::string gender, std::string address, std::string phone, std::string medical_history)
{
    if (age <= 0 || name.empty() || gender.empty() || address.empty() || phone.empty() || medical_history.empty())
    {
        return "";
    }
    const std::string patient_id = this->uuidGenerator();
    const bool result = this->repository->insert({{1, name},
                                                  {2, std::to_string(age)},
                                                  {3, gender},
                                                  {4, address},
                                                  {5, phone},
                                                  {6, medical_history},
                                                  {7, patient_id}});

    return patient_id;
}

std::string PatientService::uuidGenerator()
{
    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuid_string[37];
    uuid_unparse(uuid, uuid_string);
    return uuid_string;
}