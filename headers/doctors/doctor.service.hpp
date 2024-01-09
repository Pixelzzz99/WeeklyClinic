#pragma once
#include <memory>
#include "doctor.repository.hpp"
#include "patient.repository.hpp"
#include "prescription.repository.hpp"
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <iomanip>

class DoctorService
{
    std::shared_ptr<DoctorRepository> doctorsRepository;
    std::shared_ptr<PatientRepository> patientsRepository;
    std::shared_ptr<PrescriptionRepository> prescriptionRepository;

    std::string getTodayDate();

public:
    bool existsDoctor(std::string doctor_id);
    bool existsPatient(std::string patient_id);
    DoctorService(std::shared_ptr<DoctorRepository> repository,
                  std::shared_ptr<PatientRepository> patientsRepository,
                  std::shared_ptr<PrescriptionRepository> prescriptionRepository);
    std::string registerDoctor(std::string name, std::string specialization, std::string address, std::string phone);
    bool updatePatientMedicalHistory(std::string patient_id, std::string medical_history);
    std::unordered_map<std::string, std::string> prescribeMedication(std::string doctor_id, std::string patient_id, std::string prescription);
};
