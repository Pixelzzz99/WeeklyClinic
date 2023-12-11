#include "crow.h"
#include <iostream>
#include <string>
#include <memory>

#include "patient.repository.hpp"
#include "patient.service.hpp"
#include "patient.controller.hpp"

#include "appointment.repository.hpp"
#include "appointment.service.hpp"

#include "doctor.repository.hpp"
#include "doctor.service.hpp"
#include "doctor.controller.hpp"

#include "SqliteORM.hpp"

int main()
{
  crow::SimpleApp app;

  SqliteORM *orm = new SqliteORM();
  orm->connect("clinic.db");
  std::shared_ptr<PatientRepository> patients = std::make_shared<PatientRepository>(orm);
  std::shared_ptr<PatientService> patientService = std::make_shared<PatientService>(patients);

  std::shared_ptr<AppointmentRepository> appointments = std::make_shared<AppointmentRepository>(orm);
  std::shared_ptr<AppointmentService> appointmentService = std::make_shared<AppointmentService>(appointments);

  std::shared_ptr<PatientController> patientController = std::make_shared<PatientController>(app, patientService, appointmentService);

  std::shared_ptr<DoctorRepository> doctors = std::make_shared<DoctorRepository>(orm);
  std::shared_ptr<DoctorService> doctorService = std::make_shared<DoctorService>(doctors);
  std::shared_ptr<DoctorController> doctorController = std::make_shared<DoctorController>(app, doctorService);

  // auto appointment = orm->createEntity("appointment", {{"id", columnTypes::INT, true},
  //                                                      {"patient_id", columnTypes::TEXT, false, true, "patients", "patient_ID"},
  //                                                      {"doctor_id", columnTypes::INT, false, true, "doctors", "id"},
  //                                                      {"dateAppointment", columnTypes::TEXT},
  //                                                      {"timeAppointment", columnTypes::TEXT}});

  app.port(18080).multithreaded().run();

  return 0;
}
