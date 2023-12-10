#include "doctor.controller.hpp"
#include "doctor.service.hpp"

DoctorController::DoctorController(crow::SimpleApp &app, std::shared_ptr<DoctorService> service) : app(app), doctorService(service)
{
    setupRoutes();
}

void DoctorController::setupRoutes()
{
    CROW_ROUTE(this->app, "/doctor/register").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                              { this->registerDoctor(req, res); });
}

void DoctorController::registerDoctor(const crow::request &req, crow::response &res)
{
    res.end();
}