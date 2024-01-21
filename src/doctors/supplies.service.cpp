#include "supplies.service.hpp"
#include <iostream>

SuppliesService::SuppliesService(std::shared_ptr<SuppliesRepository> repository) : suppliesRepository(repository) {}

bool SuppliesService::addSupplies(std::vector<Supplie> values)
{
    if (values.size() == 0)
    {
        return false;
    }

    for (auto value : values)
    {

        auto supplies = suppliesRepository->select({{"name='" + value.name + "'"}});
        if (supplies.size() == 0)
        {
            if (!suppliesRepository->insert({{1, value.name},
                                             {2, std::to_string(value.quantity)},
                                             {3, std::to_string(value.price)}}))
            {
                return false;
            }
        }
        else
        {
            auto supplie = supplies[0];
            if (!this->updateSupplies(std::stoi(supplie["id"]), value.quantity))
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<Supplie> SuppliesService::getSupplies()
{
    auto supplies = suppliesRepository->select({});

    if (supplies.size() == 0)
    {
        return {};
    }

    std::vector<Supplie> result;

    for (auto supplie : supplies)
    {
        result.push_back({supplie["name"],
                          std::stoi(supplie["quantity"]),
                          std::stof(supplie["price"])});
    }

    return result;
}

std::vector<Supplie> SuppliesService::getSupplies(std::string name)
{
    auto supplies = suppliesRepository->select({{"name='" + name + "'"}});

    if (supplies.size() == 0)
    {
        return {};
    }

    std::vector<Supplie> result;

    for (auto supplie : supplies)
    {
        result.push_back({supplie["name"],
                          std::stoi(supplie["quantity"]),
                          std::stof(supplie["price"])});
    }

    return result;
}

bool SuppliesService::useSupplies(std::string name, int quantity)
{
    auto supplies = suppliesRepository->select({{"name='" + name + "'"}});

    if (supplies.size() == 0)
    {
        return false;
    }

    int id = std::stoi(supplies[0]["id"]);

    if (!this->updateSupplies(id, -quantity))
    {
        return false;
    }

    return true;
}

std::unordered_map<std::string, int> SuppliesService::checkInventory()
{
    auto supplies = suppliesRepository->select({});

    if (supplies.size() == 0)
    {
        return {};
    }

    std::unordered_map<std::string, int> result;

    for (auto supplie : supplies)
    {
        result[supplie["name"]] = std::stoi(supplie["quantity"]);
    }

    return result;
}

bool SuppliesService::updateSupplies(int id, int quantity)
{

    auto supplies = suppliesRepository->select({{"id=" + std::to_string(id)}});
    if (supplies.size() == 0)
    {
        return false;
    }

    auto supplie = supplies[0];

    return suppliesRepository->update(id, {{"quantity", std::to_string(std::stoi(supplie["quantity"]) + quantity)}});
}