#pragma once
#include "supplies.repository.hpp"
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <memory>

struct Supplie
{
    std::string name;
    int quantity;
    float price;
};

class SuppliesService
{
    std::shared_ptr<SuppliesRepository> suppliesRepository;

    bool updateSupplies(int id, int quantity);

public:
    SuppliesService(std::shared_ptr<SuppliesRepository> repository);

    bool addSupplies(std::vector<Supplie> values);

    std::vector<Supplie> getSupplies();
    std::vector<Supplie> getSupplies(std::string name);

    bool useSupplies(std::string name, int quantity);

    std::unordered_map<std::string, int> checkInventory();
};