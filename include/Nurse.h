#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_NURSE_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_NURSE_H

#include "HealthCareProfessional.h"
#include "Patient.h"

class Nurse : public HealthCareProfessional {
public:
    Nurse() = default;

    Nurse(int id, const string &name)
            : HealthCareProfessional(id, name, "Nurse") {}

    // Treats the patient and displays medication information
    void work(Patient& patient) {
        cout << "Nurse " << getName() << " treats patient " << patient.getName() << endl;
        cout << "Medication: " << patient.getMedication() << endl;
    }

};

#endif // HOSPITAL_MANAGEMENT_SYSTEM_OOP_NURSE_H
