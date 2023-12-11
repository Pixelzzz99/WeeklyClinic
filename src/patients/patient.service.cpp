#include "patient.service.hpp"
#include <uuid/uuid.h>
#include <iostream>

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
                                                  {7, std::to_string(0)},
                                                  {8, patient_id}});

    return patient_id;
}

std::string PatientService::uuidGenerator()
{
    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuid_string[37];
    uuid_unparse(uuid, uuid_string);
    std::cout << "uuid: " << uuid_string << std::endl;
    return uuid_string;
}

std::unordered_map<std::string, std::string> PatientService::getMedicalHistory(std::string patient_id)
{
    if (patient_id == "")
    {
        return {};
    }
    auto patients = this->repository->select({"patient_id = '" + patient_id + "'"});

    if (patients.empty())
    {
        return {};
    }

    auto patient = patients[0];

    if (patient.empty())
    {
        return {};
    }

    return {{"patient_id", patient["patient_ID"]},
            {"patient_name", patient["name"]},
            {"medical_history", patient["medical_history"]}};
}