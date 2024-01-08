#include "payment.service.hpp"
#include <uuid/uuid.h>
#include <iostream>
#include <regex>
#include <chrono>
#include <sstream>
#include <thread>

PaymentService::PaymentService(std::shared_ptr<PaymentRepository> repository) : repository(repository)
{
}

PaymentService::~PaymentService()
{
}

std::unordered_map<std::string, std::string> createPayment(double amount)
{
    if (amount <= 0)
    {
        throw std::runtime_error("Invalid amount");
    }

    const std::string payment_id = this->generatePaymentId();
    const std::string createdTime = this->currentDateTime();

    const bool result = repository->insert({
        {1, payment_id},
        {2, std::to_string(amount)},
        {3, std::to_string("")},
        {4, std::to_string("")},
        {5, createdTime},
        {6, std::to_string("")},
        {7, paymentStatus::PENDING},
    });

    if (!result)
    {
        throw std::runtime_error("Failed to create payment");
    }

    return
    {
        {"payment_id", payment_id},
    }
}

std::unordered_map<std::string, std::string> PaymentService::payment(std::string payment_id, std::string pan, std::string cvv, std::string expiry_date)
{

    if (!this->isCardValid(pan, cvv, expiry_date))
    {
        throw std::runtime_error("Invalid card details");
    }

    const std::string paymentDate = this->currentDateTime();

    auto payments = repository->select({{"payment_id=" + payment_id}});

    if (!this->withdraw(std::stod(payments[0]["amount"]), pan))
    {
        const bool result = repository->update(id, {
                                                       {3, pan},
                                                       {4, expiry_date},
                                                       {6, paymentDate},
                                                       {7, paymentStatus::FAILED},
                                                   });

        return
        {
            {"payment_id", payment_id},
                {"status", paymentStatus::FAILED},
        }
    }

    const bool result = repository->update(id, {
                                                   {3, pan},
                                                   {4, expiry_date},
                                                   {6, paymentDate},
                                                   {7, paymentStatus::SUCCESS},
                                               });

    if (!result)
    {
        const bool result = repository->update(id, {
                                                       {3, pan},
                                                       {4, expiry_date},
                                                       {6, paymentDate},
                                                       {7, paymentStatus::FAILED},
                                                   });

        return
        {
            {"payment_id", payment_id},
                {"status", paymentStatus::FAILED},
        }
    }

    return
    {
        {"payment_id", payment_id},
            {"paymentDate", paymentDate},
            {"status", paymentStatus::SUCCESS},
    }
}

bool PaymentService::withdraw(double amount, std::string pan)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return true;
}

std::unordered_map<std::string, std::string> PaymentService::getPaymentStatus(std::string payment_id)
{
    return {
        {"status", repository->getPaymentStatusFromDB(payment_id)},
    };
}

std::string PaymentService::getPaymentStatusFromDB(std::string payment_id)
{
    auto payments = repository->select({{"payment_id", payment_id}});
    if (payments.size() > 0)
    {
        return payments[0]["status"];
    }
    return "This payment id doesn't exist";
}

std::string PaymentService::generatePaymentId()
{
    return uuids::to_string(uuids::uuid_system_generator{}());
}

std::string PaymentService::currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

bool PaymentService::isCardValid(std::string pan, std::string cvv, std::string expiry_date)
{
    std::regex cardPattern("^[0-9]{16}$");
    std::regex cvvPattern("^[0-9]{3}$");
    std::regex expiryDatePattern("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[012])-[0-9]{4}$");
    return std::regex_match(pan, cardPattern) && std::regex_match(cvv, cvvPattern) && std::regex_match(expiry_date, expiryDatePattern);
}