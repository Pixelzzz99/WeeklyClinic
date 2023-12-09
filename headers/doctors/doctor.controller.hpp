#pragma once
#include "crow.h"
#include "doctor.service.hpp"

class DoctorController
{
    crow::SimpleApp &app;
    DoctorService *doctorService;
    void registerDoctor(const crow::request &req, crow::response &res);

public:
    DoctorController(crow::SimpleApp &app);
    void setupRoutes();
};
