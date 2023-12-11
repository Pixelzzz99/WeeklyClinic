#pragma once
#include <memory>
#include <unordered_map>
#include "IService.hpp"
#include "patient.repository.hpp"

class PatientService : public IService
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

    std::unordered_map<std::string, std::string> getMedicalHistory(std::string patient_id);
};
