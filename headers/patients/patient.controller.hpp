#pragma once
#include "crow.h"
#include "patient.service.hpp"

class PatientController
{
    crow::SimpleApp &app;
    PatientService *patientService;
    void registerPatient(const crow::request &req, crow::response &res);

public:
    PatientController(crow::SimpleApp &app);
    void setupRoutes();
};
