#pragma once
#include "crow.h"
#include "doctor.service.hpp"
#include <memory>

class DoctorController
{
    crow::SimpleApp &app;
    std::shared_ptr<DoctorService> doctorService;
    void registerDoctor(const crow::request &req, crow::response &res);
    void prescribeMedication(const crow::request &req, crow::response &res);

public:
    DoctorController(crow::SimpleApp &app, std::shared_ptr<DoctorService> service);
    void setupRoutes();
};
