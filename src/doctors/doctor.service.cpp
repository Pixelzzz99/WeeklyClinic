#include "doctor.service.hpp"
#include <iostream>

DoctorService::DoctorService(std::shared_ptr<DoctorRepository> repository,
                             std::shared_ptr<PatientRepository> patientRepository,
                             std::shared_ptr<PrescriptionRepository> prescriptionRepository,
                             std::shared_ptr<SuppliesService> suppliesService) : doctorsRepository(repository),
                                                                                 patientsRepository(patientRepository),
                                                                                 prescriptionRepository(prescriptionRepository),
                                                                                 suppliesService(suppliesService) {}

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

std::unordered_map<std::string, std::string> DoctorService::prescribeMedication(std::string doctor_id, std::string patient_id, std::string prescription, int count)
{
    if (doctor_id.empty() || patient_id.empty() || prescription.empty())
    {
        return {
            {"status", "failed"},
            {"message", "doctor_id, patient_id, prescription cannot be empty"}};
    }

    if (!this->existsDoctor(doctor_id) || !this->existsPatient(patient_id))
    {
        return {
            {"status", "failed"},
            {"message", "doctor_id or patient_id does not exist"}};
    }

    auto supplies = this->suppliesService->getSupplies(prescription);

    if (supplies.empty())
    {
        return {
            {"status", "failed"},
            {"message", "prescription not found"},
        };
    }
    const auto &supply = supplies.front();

    if (!this->prescriptionRepository->insert({{1, patient_id},
                                               {2, doctor_id},
                                               {3, supply.name},
                                               {4, this->getTodayDate()}}))
    {
        return {
            {"status", "failed"},
            {"message", "prescription not found"},
        };
    }

    if (!this->checkInventoryOrAlerts())
    {
        return {
            {"status", "failed"},
            {"message", "prescription cant be used"},
        };
    }

    if (!this->suppliesService->useSupplies(prescription, count))
    {
        return {
            {"status", "failed"},
            {"message", "prescription cant be used"},
        };
    }

    if (!this->updatePatientMedicalHistory(patient_id,
                                           prescription + " " + std::to_string(count) + " counts"))

    {
        return {
            {"status", "failed"},
            {"message", "patient medical history not updated"},
        };
    }

    return {
        {"status", "success"},
        {"patient_id", patient_id},
        {"prescription", "Patient has been prescribed: " + prescription},
    };
}

bool DoctorService::updatePatientMedicalHistory(std::string patient_id, std::string medical_history)
{
    if (patient_id.empty() || medical_history.empty())
    {
        return false;
    }

    if (!this->existsPatient(patient_id))
    {
        return false;
    }

    auto patient = this->patientsRepository->select({"patient_ID='" + patient_id + "'"})[0];

    std::string new_count_visits = std::to_string(std::stoi(patient["count_visits"]) + 1);
    std::string new_medical_history = patient["medical_history"] + "\n" + "Date: " + this->getTodayDate() + "\n" + "started taking: " + medical_history;

    auto resultInsert = this->patientsRepository->update(std::stoi(patient["id"]), {{"count_visits", new_count_visits},
                                                                                    {"medical_history", new_medical_history}});

    return resultInsert;
}

bool DoctorService::existsDoctor(std::string doctor_id)
{
    auto doctors = this->doctorsRepository->select({"id='" + doctor_id + "'"});
    return doctors.size() > 0;
}

bool DoctorService::existsPatient(std::string patient_id)
{
    auto patients = this->patientsRepository->select({"patient_ID='" + patient_id + "'"});
    return patients.size() > 0;
}

std::string DoctorService::getTodayDate()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%d-%m-%Y");
    return ss.str();
}

void DoctorService::alertAllDoctors(const std::string &message)
{
    auto doctors = doctorsRepository->select({});

    for (auto &doctor : doctors)
    {
        const std::string doctorName = doctor["name"];
        std::cout << "Alerting: " << doctorName << " with message: " << message << std::endl;
    }
}

bool DoctorService::checkInventoryOrAlerts()
{
    const auto &inventory = this->suppliesService->checkInventory();
    for (const auto &supplie : inventory)
    {
        if (supplie.second < 5)
        {
            this->alertAllDoctors("Low inventory for: " + supplie.first);
            return true;
        }
        if (supplie.second <= 0)
        {
            this->alertAllDoctors("No inventory for: " + supplie.first);
            return false;
        }
    }
    return true;
}
