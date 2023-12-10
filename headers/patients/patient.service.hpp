#pragma once
#include "patient.repository.hpp"
#include <memory>

class PatientService
{
    std::shared_ptr<PatientRepository> repository;
    std::string uuidGenerator();

public:
    PatientService(std::shared_ptr<PatientRepository> repository);
    std::string registerPatient(
        std::string name,
        int age,
        std::string gender,
        std::string address,
        std::string phone,
        std::string medical_history);
};
