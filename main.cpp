#include "crow.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <SqliteORM.hpp>

sqlite3 *db;

void initDB()
{
  int rc = sqlite3_open("./clinic.db", &db);

  if (rc)
  {
    std::cout << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    exit(1);
  }

  const std::string createPatientsTable = "CREATE TABLE IF NOT EXISTS patients (id INTEGER PRIMARY KEY, name TEXT, age INTEGER, gender TEXT, address TEXT, phone TEXT, medical_historey TEXT)";

  const std::string createAppointmentsTable = "CREATE TABLE IF NOT EXISTS appointments (id INTEGER PRIMARY KEY, patient_id INTEGER, date TEXT, time INTEGER, doctor_id INTEGER)";

  const std::string createDoctorsTable = "CREATE TABLE IF NOT EXISTS doctors (id INTEGER PRIMARY KEY, name TEXT, specialization TEXT, address TEXT, phone TEXT)";

  sqlite3_exec(db, createPatientsTable.c_str(), 0, 0, 0);
  sqlite3_exec(db, createDoctorsTable.c_str(), 0, 0, 0);
  sqlite3_exec(db, createAppointmentsTable.c_str(), 0, 0, 0);
}

void registerPatient(const crow::request &req, crow::response &res)
{
  try
  {
    auto json = crow::json::load(req.body);
    res.add_header("Content-Type", "application/json");

    if (!json)
    {
      res.code = 400;
      throw std::runtime_error("Invalid JSON");
    }

    const std::string name = json["name"].s();
    const int age = json["age"].i();
    std::cout << "Age: " << age << std::endl;
    const std::string gender = json["gender"].s();
    const std::string address = json["address"].s();
    const std::string phone = json["phone"].s();
    const std::string medical_history = json["medical_history"].s();
    std::cout << "HERE" << std::endl;

    if (name.empty() || age < 0 || gender.empty() || address.empty() || phone.empty() || medical_history.empty())
    {
      res.code = 400;
      throw std::runtime_error("Required fields are empty: name, age, gender, address, phone, medical_historey");
    }

    sqlite3_stmt *stmt;
    const std::string insertPatientQuery = "INSERT INTO patients (name, age, gender, address, phone, medical_historey) VALUES (?,?,?,?,?,?)";
    int rc = sqlite3_prepare_v2(db, insertPatientQuery.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
      res.code = 500;
      throw std::runtime_error("Internal server error");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), name.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, age);
    sqlite3_bind_text(stmt, 3, gender.c_str(), gender.size(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, address.c_str(), address.size(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, phone.c_str(), phone.size(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, medical_history.c_str(), medical_history.size(), SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
      std::cerr << "Failed to insert patient: " << sqlite3_errmsg(db) << std::endl;
      res.code = 500;
      throw std::runtime_error("Internal server error");
    }

    sqlite3_finalize(stmt);
    crow::json::wvalue result = crow::json::wvalue::object();
    res.code = 200;
    result["id"] = sqlite3_last_insert_rowid(db);
    result["message"] = "Patient registered successfully";
    res.write(result.dump());
    res.end();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    crow::json::wvalue error = crow::json::wvalue::object();
    error["error"] = e.what();
    res.write(error.dump());
    res.end();
  }
}

int main()
{
  // initDB();
  // crow::SimpleApp app; // define your crow application

  // // define your endpoint at the root directory
  // CROW_ROUTE(app, "/register_patient").methods(crow::HTTPMethod::POST)(registerPatient);

  // // set the port, set the app to run on multiple threads, and run the app
  // app.port(18080).multithreaded().run();

  // sqlite3_close(db);
  SqliteORM *orm = new SqliteORM();
  orm->connect("clinic.db");
  orm->createEntity("patients", {
                                    {"id", columnTypes::INT, true},
                                    {"name", columnTypes::TEXT},
                                    {"age", columnTypes::INT},
                                    {"gender", columnTypes::TEXT},
                                    {"address", columnTypes::TEXT},
                                    {"phone", columnTypes::TEXT},
                                    {"medical_history", columnTypes::TEXT},
                                });

  return 0;
}
