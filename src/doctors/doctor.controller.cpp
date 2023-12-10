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
    crow::json::rvalue json = crow::json::load(req.body);
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
        const std::string specialization = json["specialization"].s();
        const std::string address = json["address"].s();
        const std::string phone = json["phone"].s();

        if (name.empty() || specialization.empty() || address.empty() || phone.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: name, specialization, address, phone");
        }

        const std::string doctor_name = this->doctorService->registerDoctor(name, specialization, address, phone);

        if (name == "")
        {
            res.code = 500;
            throw std::runtime_error("Internal server error");
        }

        res.code = 200;
        result["message"] = "Doctor registered successfully";
        result["doctorName"] = doctor_name;
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