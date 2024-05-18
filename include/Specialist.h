#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_SPECIALIST_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_SPECIALIST_H

#include <utility>

#include "HealthCareProfessional.h"
#include "Nurse.h"

class Specialist : public HealthCareProfessional {
public:
    Specialist() = default;

    Specialist(int id, const string& name, const string& field)
            : HealthCareProfessional(id, name, field) {}

    // Performs specialized treatment for the patient
    void work(Patient& patient) {
        string patientDescription, patientTreatment, treatmentName;
        cout << "Enter patient description: ";
        getline(cin, patientDescription);
        cout << "Enter treatment name: ";
        getline(cin, patientTreatment);
        cout << "Enter patient treatment: ";
        getline(cin, patientTreatment);
        patient.setTreatment(name, patientDescription, patientTreatment);
        cout << "Patient treatment described by specialist: " << patient.getTreatment().treatment << endl;
    }

    // Requests assistance from available nurses
    void requestAssist(vector<Nurse>& nurses, const string& time) {
        cout << "Requesting assist from nurse" << endl;
        Nurse* availableNurse = findAvailableNurse(nurses, time);

        if (availableNurse == nullptr) {
            cout << "No nurse available" << endl;
            return;
        }

        cout << "Nurse " << availableNurse->getName() << " is assisting " << specialization << " " << name << endl;
    }

    // Finds an available nurse for assistance
    static Nurse* findAvailableNurse(vector<Nurse>& nurses, const string& time) {
        for (Nurse& nurse : nurses) {
            if (nurse.schedule.isAvailable(time)) {
                return &nurse;
            }
        }
        return nullptr;
    }

};

#endif // HOSPITAL_MANAGEMENT_SYSTEM_OOP_SPECIALIST_H
