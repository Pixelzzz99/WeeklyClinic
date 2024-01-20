#include "doctor.service.hpp"
#include <string>

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

std::unordered_map<std::string, std::string> DoctorService::prescribeMedication(std::string doctor_id, std::string patient_id, std::string prescription)
{
    if (doctor_id.empty() || patient_id.empty() || prescription.empty())
    {
        return {};
    }

    if (!this->existsDoctor(doctor_id) || !this->existsPatient(patient_id))
    {
        return {};
    }

    // TODO: check prescriptions and supplies

    auto resultInsert = this->prescriptionRepository->insert({{1, patient_id},
                                                              {2, doctor_id},
                                                              {3, prescription},
                                                              {4, this->getTodayDate()}});
    if (!resultInsert)
    {
        return {};
    }

    bool isPatientDataUpdated = this->updatePatientMedicalHistory(patient_id, prescription);

    if (!isPatientDataUpdated)
    {
        return {};
    }

    return {
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

bool DoctorService::alertAllDoctors(std::string message)
{
    // TODO: implement
}

bool DoctorService::checkInventoryOrAlerts()
{
    // TODO: implement
    return true;
}