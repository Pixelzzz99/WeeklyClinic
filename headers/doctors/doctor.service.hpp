#pragma once
#include "doctor.repository.hpp"
#include <memory>

class DoctorService
{
    std::shared_ptr<DoctorRepository> doctorsRepository;

public:
    DoctorService();
    void registerDoctor();
};
