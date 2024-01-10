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

#include "payment.repository.hpp"
#include "payment.service.hpp"

#include "SqliteORM.hpp"

int main()
{
  crow::SimpleApp app;

  SqliteORM *orm = new SqliteORM();
  orm->connect("clinic.db");
  std::shared_ptr<PatientRepository> patients = std::make_shared<PatientRepository>(orm);
  std::shared_ptr<DoctorRepository> doctors = std::make_shared<DoctorRepository>(orm);
  std::shared_ptr<AppointmentRepository> appointments = std::make_shared<AppointmentRepository>(orm);
  std::shared_ptr<PrescriptionRepository> prescriptions = std::make_shared<PrescriptionRepository>(orm);
  std::shared_ptr<PaymentRepository> payments = std::make_shared<PaymentRepository>(orm);

  std::shared_ptr<PatientService> patientService = std::make_shared<PatientService>(patients);
  std::shared_ptr<PaymentService> paymentService = std::make_shared<PaymentService>(payments);
  std::shared_ptr<AppointmentService> appointmentService = std::make_shared<AppointmentService>(appointments, paymentService);
  std::shared_ptr<DoctorService> doctorService = std::make_shared<DoctorService>(doctors, patients, prescriptions);

  std::shared_ptr<PatientController> patientController = std::make_shared<PatientController>(app, patientService, appointmentService);
  std::shared_ptr<DoctorController> doctorController = std::make_shared<DoctorController>(app, doctorService);

  app.port(18080).multithreaded().run();

  return 0;
}
