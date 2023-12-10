#include "doctor.service.hpp"
#include "doctor.repository.hpp"

DoctorService::DoctorService(std::shared_ptr<DoctorRepository> repository) : doctorsRepository(repository) {}

void DoctorService::registerDoctor()
{
}