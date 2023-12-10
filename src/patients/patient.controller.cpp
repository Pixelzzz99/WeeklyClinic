#include "patient.service.hpp"
#include "patient.controller.hpp"

PatientController::PatientController(crow::SimpleApp &app, std::shared_ptr<PatientService> service) : app(app), patientService(service)
{
    setupRoutes();
}

void PatientController::setupRoutes()
{
    CROW_ROUTE(this->app, "/patient/register").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                               { this->registerPatient(req, res); });
}

void PatientController::registerPatient(const crow::request &req, crow::response &res)
{
    auto json = crow::json::load(req.body);
    res.add_header("Content-Type", "application/json");
    crow::json::wvalue result = crow::json::wvalue::object();
    try
    {
        if (!json)
        {
            res.code = 400;
            throw std::runtime_error("Invalid JSON");
        }

        const std::string name = json["name"].s();
        const int age = json["age"].i();
        const std::string gender = json["gender"].s();
        const std::string address = json["address"].s();
        const std::string phone = json["phone"].s();
        const std::string medical_history = json["medical_history"].s();

        if (name.empty() || age < 0 || gender.empty() || address.empty() || phone.empty() || medical_history.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: name, age, gender, address, phone, medical_historey");
        }

        const std::string patient_id = this->patientService->registerPatient(name, age, gender, address, phone, medical_history);

        if (patient_id == "")
        {
            res.code = 500;
            throw std::runtime_error("Internal server error");
        }

        res.code = 200;
        result["message"] = "Patient registered successfully";
        result["id"] = patient_id;
        res.body = result.dump();
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        result["message"] = e.what();
        res.body = result.dump();
        res.end();
    }
}