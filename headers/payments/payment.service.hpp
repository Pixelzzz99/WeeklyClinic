#pragma once
#include "payment.repository.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace paymentStatus
{
    const std::string SUCCESS = "SUCCESS";
    const std::string FAILED = "FAILED";
    const std::string PENDING = "PENDING";
}

class PaymentService
{
public:
    PaymentService(std::shared_ptr<PaymentRepository> repository);

    std::unordered_map<std::string, std::string> createPayment(int amount);

    std::unordered_map<std::string, std::string> getPaymentStatus(std::string payment_id);

    std::unordered_map<std::string, std::string> payment(std::string payment_id, std::string pan, std::string cvv, std::string expiry_date);

private:
    bool withdraw(double amount, std::string pan);

    bool isCardValid(std::string pan, std::string cvv, std::string expiry_date);

    std::string getPaymentStatusFromDB(std::string payment_id);

    std::string generatePaymentId();

    std::string currentDateTime();
};