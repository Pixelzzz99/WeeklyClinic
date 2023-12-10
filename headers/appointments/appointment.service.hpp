#pragma once
#include <unordered_map>
#include "IService.hpp"
#include "appointment.repository.hpp"

class AppointmentService : public IService
{

    bool isValidDate(std::string date);
    bool isValidTime(std::string time);
    bool isDateInFuture(const std::string &date);
    std::shared_ptr<AppointmentRepository> repository;

public:
    AppointmentService(std::shared_ptr<AppointmentRepository> repository);

    std::string bookingPatient(std::string patient_id, int doctor_id, std::string date, std::string time);

    bool cancelAppointment(int id);
};
