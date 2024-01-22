#include "supplies.controller.hpp"

SuppliesController::SuppliesController(crow::SimpleApp &app, std::shared_ptr<SuppliesService> service) : app(app), suppliesService(service)
{
    setupRoutes();
}

void SuppliesController::setupRoutes()
{
    CROW_ROUTE(this->app, "/supplies/").methods(crow::HTTPMethod::POST)([&](const crow::request &req, crow::response &res)
                                                                        { this->addSupplies(req, res); });

    CROW_ROUTE(this->app, "/supplies/").methods(crow::HTTPMethod::GET)([&](const crow::request &req, crow::response &res)
                                                                       { this->getSupplies(req, res); });
}

void SuppliesController::getSupplies(const crow::request &req, crow::response &res)
{

    res.add_header("Content-Type", "application/json");
    crow::json::wvalue result = crow::json::wvalue::object();
    try
    {
        res.code = 200;
        auto supplies = this->suppliesService->getSupplies();
        if (supplies.empty())
        {
            res.code = 404;
            throw std::runtime_error("Supplies not found");
        }
        for (auto i = 0; i < supplies.size(); i++)
        {
            result["supplies"][i]["name"] = supplies[i].name;
            result["supplies"][i]["quantity"] = supplies[i].quantity;
            result["supplies"][i]["price"] = supplies[i].price;
        }

        res.body = result.dump();
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        result["message"] = e.what();
        res.body = result.dump();
        res.end();
    }
}

void SuppliesController::addSupplies(const crow::request &req, crow::response &res)
{
    auto json = crow::json::load(req.body);
    res.add_header("Content-Type", "application/json");
    crow::json::wvalue result = crow::json::wvalue::object();

    try
    {
        if (!json)
        {
            res.code = 400;
            throw std::runtime_error("Invalid JSON");
        }

        auto values = json["supplies"];
        std::vector<Supplie> newSupplies = {};

        for (auto value : values)
        {
            const std::string name = value["name"].s();
            const int quantity = value["quantity"].i();
            const float price = value["price"].d();
            if (name.empty() || !quantity || !price)
            {
                res.code = 400;
                throw std::runtime_error("Required fields are empty: name, quantity, price");
            }

            newSupplies.push_back(Supplie{name, quantity, price});
        }

        this->suppliesService->addSupplies(newSupplies);

        res.code = 201;
        result["message"] = "Supplies added successfully";
        res.body = result.dump();
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        result["message"] = e.what();
        res.body = result.dump();
        res.end();
    }
}
