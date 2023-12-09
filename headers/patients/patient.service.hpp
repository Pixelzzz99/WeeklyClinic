#pragma once
#include "patient.repository.hpp"
#include <memory>

class PatientService
{
    std::shared_ptr<PatientRepository> repository;

public:
    PatientService();
    void registerPatient();
};
