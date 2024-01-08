#include "payment.repository.hpp"

PaymentRepository::PaymentRepository(SqliteORM *orm) : orm(orm)
{
    this->payments = this->orm->createEntity("payments", {{"id", columnTypes::INT, true},
                                                          {"payment_id", columnTypes::TEXT},
                                                          {"amount", columnTypes::DOUBLE},
                                                          {"pan", columnTypes::TEXT},
                                                          {"expired", columnTypes::TEXT},
                                                          {"createdDate", columnTypes::TEXT},
                                                          {"paymentDate", columnTypes::TEXT},
                                                          {"status", columnTypes::TEXT}});
}

PaymentRepository::~PaymentRepository()
{
}

std::vector<std::map<std::string, std::string>> PaymentRepository::select(std::vector<std::string> conditions)
{
    return this->payments->select(conditions);
}

bool PaymentRepository::insert(std::vector<std::pair<int, std::string>> values)
{
    return this->payments->insert(values);
}

bool PaymentRepository::update(int id, std::vector<std::pair<std::string, std::string>> values)
{
    return this->payments->update(id, values);
}

bool PaymentRepository::delete_(int id)
{
    return this->payments->delete_(id);
}
