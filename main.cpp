#include "crow.h"
#include <iostream>
#include <string>
#include <memory>

#include "patient.repository.hpp"
#include "patient.service.hpp"
#include "patient.controller.hpp"

#include "prescription.repository.hpp"

#include "appointment.repository.hpp"
#include "appointment.service.hpp"

#include "doctor.repository.hpp"
#include "doctor.service.hpp"
#include "doctor.controller.hpp"
#include "supplies.repository.hpp"
#include "supplies.service.hpp"

#include "payment.repository.hpp"
#include "payment.service.hpp"

#include "SqliteORM.hpp"

int main()
{
  crow::SimpleApp app;

  SqliteORM *orm = new SqliteORM();
  orm->connect("clinic.db");
  // std::shared_ptr<PatientRepository> patients = std::make_shared<PatientRepository>(orm);
  // std::shared_ptr<DoctorRepository> doctors = std::make_shared<DoctorRepository>(orm);
  // std::shared_ptr<AppointmentRepository> appointments = std::make_shared<AppointmentRepository>(orm);
  // std::shared_ptr<PrescriptionRepository> prescriptions = std::make_shared<PrescriptionRepository>(orm);
  // std::shared_ptr<PaymentRepository> payments = std::make_shared<PaymentRepository>(orm);
  // std::shared_ptr<SuppliesRepository> supplies = std::make_shared<SuppliesRepository>(orm);

  // std::shared_ptr<PatientService> patientService = std::make_shared<PatientService>(patients);
  // std::shared_ptr<PaymentService> paymentService = std::make_shared<PaymentService>(payments);
  // std::shared_ptr<AppointmentService> appointmentService = std::make_shared<AppointmentService>(appointments, paymentService);
  // std::shared_ptr<DoctorService> doctorService = std::make_shared<DoctorService>(doctors, patients, prescriptions);

  // std::shared_ptr<PatientController> patientController = std::make_shared<PatientController>(app, patientService, appointmentService);
  // std::shared_ptr<DoctorController> doctorController = std::make_shared<DoctorController>(app, doctorService);

  // app.port(18080).multithreaded().run();

  // auto result = appointmentService->bookingPatient({"1", 1, "23-01-2024", "14:00", 100});
  // std::cout << result << std::endl;

  // auto resultPayment = appointmentService->bookingPayment({result, "1234567890123456", "123", "12/22"});
  // std::cout << resultPayment << std::endl;

  // auto result = paymentService->createPayment(100);
  // std::cout << "PAYMANT_ID: " << result["payment_id"] << std::endl;
  // auto resultPayment = paymentService->payment(result["payment_id"], "1234567890123456", "123", "12/22");

  // std::cout << resultPayment["status"] << std::endl;
  // std::cout << paymentService->getPaymentStatus(resultPayment["payment_id"])["status"] << std::endl;

  std::shared_ptr<SuppliesRepository> supplies = std::make_shared<SuppliesRepository>(orm);
  std::shared_ptr<SuppliesService> suppliesService = std::make_shared<SuppliesService>(supplies);

  auto result = suppliesService->getSupplies();
  for (auto supplie : result)
  {
    std::cout << supplie.name << " " << supplie.quantity << " " << supplie.price << std::endl;
  }

  auto inventory = suppliesService->checkInventory();
  for (auto supplie : inventory)
  {
    std::cout << supplie.first << " " << supplie.second << std::endl;
  }

  suppliesService->useSupplies("Bandage", 50);

  inventory = suppliesService->checkInventory();
  for (auto supplie : inventory)
  {
    std::cout << supplie.first << " " << supplie.second << std::endl;
  }

  return 0;
}
