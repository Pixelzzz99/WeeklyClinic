#pragma once
#include <unordered_map>
#include "IService.hpp"
#include "appointment.repository.hpp"
#include "payment.service.hpp"

struct BookingParams
{
    std::string patient_id;
    int doctor_id;
    std::string date;
    std::string time;
    double amount;
};

struct BookingPayment
{
    std::string paymant_id;
    std::string pan;
    std::string cvv;
    std::string expiry_date;
};

namespace BookingStatus
{
    const std::string VISITED = "VISITED";
    const std::string SUCCESS = "BOOKED";
    const std::string FAILED = "FAILED";
    const std::string CANCELLED = "CANCELLED";
    const std::string PENDING = "PENDING PAYMENT";
};

class AppointmentService : public IService
{
    bool isValidDate(std::string date);
    bool isValidTime(std::string time);
    bool isDateInFuture(const std::string &date);
    std::shared_ptr<AppointmentRepository> repository;
    std::shared_ptr<PaymentService> paymentService;

public:
    AppointmentService(std::shared_ptr<AppointmentRepository> repository,
                       std::shared_ptr<PaymentService> paymentService);

    std::string bookingPatient(BookingParams params);

    std::string bookingPayment(BookingPayment params);

    bool cancelAppointment(int id);
};
