#include "appointment.service.hpp"
#include <regex>
#include <chrono>
#include <sstream>
#include <iomanip>

AppointmentService::AppointmentService(std::shared_ptr<AppointmentRepository> repository,
                                       std::shared_ptr<PaymentService> paymentService) : repository(repository), paymentService(paymentService)
{
}

std::string AppointmentService::bookingPatient(BookingParams params)
{
    const std::string patient_id = params.patient_id;
    const int doctor_id = params.doctor_id;
    const std::string date = params.date;
    const std::string time = params.time;
    const double amount = params.amount;

    if (!this->isValidDate(date))
    {
        return "Date should be in format: 01-01-2023";
    }

    if (!this->isValidTime(time))
    {
        return "Time should be in format: 14:00, 15:00,.... From 07:00 to 19:00";
    }

    if (!this->isDateInFuture(date))
    {
        return "Date should be in the future";
    }

    auto apointmentExist = repository->select({"doctor_id=" + std::to_string(doctor_id), "dateAppointment='" + date + "'", "timeAppointment='" + time + "'"});

    if (apointmentExist.size() > 0)
    {
        return "This doctor is not available at this time";
    }

    auto payment = this->paymentService->createPayment(amount);
    const bool isBookingCreated = repository->insert({{1, patient_id},
                                                      {2, std::to_string(doctor_id)},
                                                      {3, date},
                                                      {4, time},
                                                      {5, BookingStatus::PENDING},
                                                      {6, std::to_string(amount)},
                                                      {7, payment["payment_id"]}});

    if (!isBookingCreated)
    {
        return "Something went wrong, please try again";
    }
    return payment["payment_id"];
}

std::string AppointmentService::bookingPayment(BookingPayment params)
{
    try
    {
        const std::string payment_id = params.paymant_id;
        const std::string pan = params.pan;
        const std::string cvv = params.cvv;
        const std::string expiry_date = params.expiry_date;

        auto payment = this->paymentService->payment(payment_id, pan, cvv, expiry_date);

        auto booking = this->repository->select({"payment_id='" + payment_id + "'"});

        if (booking.size() == 0)
        {
            throw std::runtime_error("Invalid payment id");
        }

        int id = std::stoi(booking[0]["id"]);
        std::string date = booking[0]["dateAppointment"];
        std::string time = booking[0]["timeAppointment"];

        if (payment["status"] == "FAILED")
        {
            this->repository->update(id, {
                                             {"status", BookingStatus::FAILED},
                                         });
            return "Payment failed";
        }

        this->repository->update(id, {
                                         {"status", BookingStatus::SUCCESS},
                                     });

        return "Appointment booked successfully in  " + date + " at " + time;
    }
    catch (const std::exception &e)
    {
        return "Something went wrong, please try again";
    }
}

bool AppointmentService::cancelAppointment(int id)
{
    auto booking = this->repository->select({"id=" + std::to_string(id)});

    if (booking.size() == 0)
    {
        return false;
    }

    if (booking[0]["status"] == BookingStatus::VISITED)
    {
        return false;
    }

    return repository->update(id, {{"status", BookingStatus::CANCELLED}});
}

bool AppointmentService::isValidDate(std::string date)
{
    std::regex datePattern("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[012])-[0-9]{4}$");
    return std::regex_match(date, datePattern);
}

bool AppointmentService::isValidTime(std::string time)
{
    std::regex timePattern("^(0[7-9]|1[0-9]|2[0123]):00$");
    return std::regex_match(time, timePattern);
}

bool AppointmentService::isDateInFuture(const std::string &date)
{
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%d-%m-%Y");
    auto date_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto now = std::chrono::system_clock::now();
    return date_point > now;
}