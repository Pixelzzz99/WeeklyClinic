#pragma once
#include <memory>
#include "crow.h"
#include "patient.service.hpp"

class PatientController
{
    crow::SimpleApp &app;
    std::shared_ptr<PatientService> patientService;
    void registerPatient(const crow::request &req, crow::response &res);

public:
    PatientController(crow::SimpleApp &app, std::shared_ptr<PatientService> service);
    void setupRoutes();
};
