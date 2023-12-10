#pragma once
#include <memory>
#include <unordered_map>
#include "crow.h"
#include "patient.service.hpp"
#include "appointment.service.hpp"

class PatientController
{
    crow::SimpleApp &app;
    std::shared_ptr<PatientService> patientService;
    std::shared_ptr<AppointmentService> appointmentService;
    void registerPatient(const crow::request &req, crow::response &res);
    void bookAppointment(const crow::request &req, crow::response &res);

public:
    PatientController(crow::SimpleApp &app,
                      std::shared_ptr<PatientService> patientservice,
                      std::shared_ptr<AppointmentService> appointmenservice);
    void setupRoutes();
};
