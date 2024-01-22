#include "patient.service.hpp"
#include "patient.controller.hpp"

PatientController::PatientController(crow::SimpleApp &app,
                                     std::shared_ptr<PatientService> patientservice,
                                     std::shared_ptr<AppointmentService> appointmentservice) : app(app),
                                                                                               patientService(patientservice), appointmentService(appointmentservice)
{
    setupRoutes();
}

void PatientController::setupRoutes()
{
    CROW_ROUTE(this->app, "/patient/register").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                               { this->registerPatient(req, res); });

    CROW_ROUTE(this->app, "/patient/appointment").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                                  { this->bookAppointment(req, res); });

    CROW_ROUTE(this->app, "/patient/medical_history/<string>").methods(crow::HTTPMethod::GET)([&](const crow::request &req, crow::response &res, const std::string &patient_id)
                                                                                              { this->getMedicalHistory(req, res, patient_id); });

    CROW_ROUTE(this->app, "/payment/<string>").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res, const std::string &payment_id)
                                                                               { this->bookPayment(req, res, payment_id); });
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

void PatientController::bookAppointment(const crow::request &req, crow::response &res)
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

        const std::string patient_id = json["patient_id"].s();
        const std::string doctor_id = json["doctor_id"].s();
        const std::string date = json["date"].s();
        const std::string time = json["time"].s();
        const std::string amount = json["amount"].s();

        if (patient_id.empty() || doctor_id.empty() || date.empty() || time.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: patient_id, doctor_id, date, time");
        }

        const std::string paymentId = this->appointmentService->bookingPatient({patient_id, stoi(doctor_id), date, time, stof(amount)});

        res.code = 200;
        result["paymentId"] = paymentId;
        result["url"] = "http://localhost:18080/payment/" + paymentId;
        result["message"] = "Please make payment to book appointment";
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

void PatientController::bookPayment(const crow::request &req, crow::response &res, const std::string &paymentId)
{
    auto json = crow::json::load(req.body);
    res.add_header("Content-Type", "application/json");
    crow::json::wvalue result = crow::json::wvalue::object();

    try
    {
        if (paymentId.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: paymentId");
        }

        const std::string pan = json["pan"].s();
        const std::string cvv = json["cvv"].s();
        const std::string expiry_date = json["expiry_date"].s();

        if (pan.empty() || cvv.empty() || expiry_date.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: pan, cvv, expiry_date");
        }

        const std::string status = this->appointmentService->bookingPayment({paymentId, pan, cvv, expiry_date});

        res.code = 200;
        result["message"] = status;
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

void PatientController::getMedicalHistory(const crow::request &req, crow::response &res, const std::string &patient_id)
{
    res.add_header("Content-Type", "application/json");
    crow::json::wvalue result = crow::json::wvalue::object();
    try
    {
        const std::string patient_Id = patient_id;

        if (patient_Id.empty())
        {
            res.code = 400;
            throw std::runtime_error("Required fields are empty: patient_id");
        }

        std::unordered_map<std::string, std::string> result_data = this->patientService->getMedicalHistory(patient_Id);

        if (result_data.empty())
        {
            res.code = 400;
            throw std::runtime_error("Invalid patient_id");
        }

        res.code = 200;
        result["patient_name"] = result_data["patient_name"];

        result["medical_history"] = result_data["medical_history"];
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