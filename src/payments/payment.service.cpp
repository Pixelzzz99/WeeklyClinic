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

std::unordered_map<std::string, std::string> PaymentService::createPayment(double amount)
{
    if (amount <= 0)
    {
        throw std::runtime_error("Invalid amount");
    }

    const std::string payment_id = this->generatePaymentId();
    const std::string createdTime = this->currentDateTime();

    const bool result = this->repository->insert({
        {1, payment_id},
        {2, std::to_string(amount)},
        {3, std::to_string(0)},
        {4, std::to_string(0)},
        {5, createdTime},
        {6, std::to_string(0)},
        {7, paymentStatus::PENDING},
    });

    if (!result)
    {
        throw std::runtime_error("Failed to create payment");
    }

    return {
        {"payment_id", payment_id},
    };
}

std::unordered_map<std::string, std::string> PaymentService::payment(std::string payment_id, std::string pan, std::string cvv, std::string expiry_date)
{

    if (!this->isCardValid(pan, cvv, expiry_date))
    {
        throw std::runtime_error("Invalid card details");
    }

    const std::string paymentDate = this->currentDateTime();

    auto payments = this->repository->select({"payment_id='" + payment_id + "'"});

    if (payments.size() == 0)
    {
        throw std::runtime_error("Invalid payment id");
    }

    int id = std::stoi(payments[0]["id"]);

    if (!this->withdraw(std::stod(payments[0]["amount"]), pan))
    {
        this->updatePaymentStatus({
            .id = id,
            .pan = pan,
            .expiry_date = expiry_date,
            .paymentStatus = paymentStatus::FAILED,
        });

        return {
            {"payment_id", payment_id},
            {"status", paymentStatus::FAILED},
        };
    }

    if (!this->updatePaymentStatus({
            .id = id,
            .pan = pan,
            .expiry_date = expiry_date,
            .paymentStatus = paymentStatus::SUCCESS,
        }))
    {
        const bool result = this->repository->update(id, {
                                                             {"status", paymentStatus::FAILED},
                                                         });

        return {
            {"payment_id", payment_id},
            {"status", paymentStatus::FAILED},
        };
    }

    return {
        {"payment_id", payment_id},
        {"paymentDate", paymentDate},
        {"status", paymentStatus::SUCCESS},
    };
}

bool PaymentService::isCardValid(std::string pan, std::string cvv, std::string expiry_date)
{
    std::regex cardPattern("^[0-9]{16}$");
    std::regex cvvPattern("^[0-9]{3}$");
    std::regex expiryDatePattern("^(0[1-9]|1[0-2])/?([0-9]{4}|[0-9]{2})$");
    return std::regex_match(pan, cardPattern) && std::regex_match(cvv, cvvPattern) && std::regex_match(expiry_date, expiryDatePattern);
}

bool PaymentService::updatePaymentStatus(PaymentUpdateParams params)
{
    std::string paymentDate = this->currentDateTime();
    return this->repository->update(params.id, {
                                                   {"pan", params.pan},
                                                   {"expired", params.expiry_date},
                                                   {"paymentDate", paymentDate},
                                                   {"status", params.paymentStatus},
                                               });
}

bool PaymentService::withdraw(double amount, std::string pan)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return false;
}

std::unordered_map<std::string, std::string> PaymentService::getPaymentStatus(std::string payment_id)
{
    return {
        {"status", this->getPaymentStatusFromDB(payment_id)},
    };
}

std::string PaymentService::getPaymentStatusFromDB(std::string payment_id)
{
    auto payments = this->repository->select({{"payment_id='" + payment_id + "'"}});
    if (payments.size() > 0)
    {
        return payments[0]["status"];
    }
    return "This payment id doesn't exist";
}

std::string PaymentService::generatePaymentId()
{
    uuid_t uuid;
    uuid_generate_random(uuid);
    char payment_id[37];
    uuid_unparse(uuid, payment_id);
    return payment_id;
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
