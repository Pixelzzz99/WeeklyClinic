#include "patient.service.hpp"
#include "patient.controller.hpp"

PatientController::PatientController(crow::SimpleApp &app) : app(app)
{
    patientService = new PatientService();
    setupRoutes();
}

void PatientController::setupRoutes()
{
    CROW_ROUTE(this->app, "/patient/register").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                               { this->registerPatient(req, res); });
}

void PatientController::registerPatient(const crow::request &req, crow::response &res)
{
    res.end();
}