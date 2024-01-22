#pragma once
#include "crow.h"
#include "supplies.service.hpp"
#include <memory>

class SuppliesController
{
    crow::SimpleApp &app;
    std::shared_ptr<SuppliesService> suppliesService;
    void getSupplies(const crow::request &req, crow::response &res);
    void addSupplies(const crow::request &req, crow::response &res);

public:
    SuppliesController(crow::SimpleApp &app, std::shared_ptr<SuppliesService> service);
    void setupRoutes();
};
