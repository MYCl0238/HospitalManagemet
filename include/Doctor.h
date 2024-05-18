#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_DOCTOR_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_DOCTOR_H

#include "HealthCareProfessional.h"
#include "Patient.h"


class Doctor : public HealthCareProfessional {
public:
    Doctor() = default;

    Doctor(int id, const string& name)
            : HealthCareProfessional(id, name, "Doctor") {}

    void work(Patient& patient) {
        string medication;
        cout << "Patient treatment described by specialist: " << patient.getTreatment().name << " " << patient.getTreatment().treatment << endl;
        cout << "Enter medication: ";
        getline(cin, medication);
        patient.setMedication(medication);
        cout << "Prescribed medication: " << medication << endl;

    }

};
#endif //HOSPITAL_MANAGEMENT_SYSTEM_OOP_DOCTOR_H
