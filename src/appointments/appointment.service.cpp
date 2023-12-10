#include "appointment.service.hpp"
#include <regex>
#include <chrono>
#include <sstream>
#include <iomanip>

AppointmentService::AppointmentService(std::shared_ptr<AppointmentRepository> repository) : repository(repository)
{
}

std::string AppointmentService::bookingPatient(std::string patient_id, int doctor_id, std::string date, std::string time)
{
    if (!isValidDate(date))
    {
        return "Date should be in format: 01-01-2023";
    }

    if (!isValidTime(time))
    {
        return "Time should be in format: 14:00, 15:00,.... From 07:00 to 19:00";
    }

    if (!isDateInFuture(date))
    {
        return "Date should be in the future";
    }

    auto apointmentExist = repository->select({"doctor_id=" + std::to_string(doctor_id), "dateAppointment='" + date + "'", "timeAppointment='" + time + "'"});

    if (apointmentExist.size() > 0)
    {
        return "This doctor is not available at this time";
    }

    const bool isBooked = repository->insert({{1, patient_id},
                                              {2, std::to_string(doctor_id)},
                                              {3, date},
                                              {4, time},
                                              {5, "booked"}});

    if (!isBooked)
    {
        return "Something went wrong, please try again";
    }

    return "Appointment booked successfully in " + date + " at " + time;
}

bool AppointmentService::cancelAppointment(int id)
{
    return repository->update(id, {{"status", "cancelled"}});
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