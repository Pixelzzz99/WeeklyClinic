#include "doctor.service.hpp"
#include "doctor.repository.hpp"

DoctorService::DoctorService(std::shared_ptr<DoctorRepository> repository) : doctorsRepository(repository) {}

std::string DoctorService::registerDoctor(std::string name, std::string specialization, std::string address, std::string phone)
{
    if (name.empty() || specialization.empty() || address.empty() || phone.empty())
    {
        return "";
    }

    this->doctorsRepository->insert({{1, name},
                                     {2, specialization},
                                     {3, address},
                                     {4, phone}});

    return name;
}