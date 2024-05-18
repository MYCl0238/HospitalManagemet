
#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_HEALTHCAREPROFESSIONAL_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_HEALTHCAREPROFESSIONAL_H

#include <utility>

#include "Schedule.h"
#include "inputHandler.h"

using namespace std;

class HealthCareProfessional {
protected:
    string specialization;
    int id;
    string name;

public:
    Schedule schedule;

    HealthCareProfessional() = default;

    HealthCareProfessional(int id, string  name, string  specialization)
            : id(id), name(std::move(name)), specialization(std::move(specialization)) {}

    string getSpecialization() const {
        return specialization;
    }

    string getName() const {
        return name;
    }

    int getId() const {
        return id;
    }

    void serialize(ofstream& ofs) {
        ofs << id << endl;
        ofs << name << endl;
        ofs << specialization << endl;
        schedule.serialize(ofs);
    }

    void deserialize(ifstream& ifs) {
        ifs >> id;
        ifs.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(ifs, name);
        getline(ifs, specialization);
        schedule.deserialize(ifs);
    }

    virtual void work(Patient& patient) = 0;
};


#endif //HOSPITAL_MANAGEMENT_SYSTEM_OOP_HEALTHCAREPROFESSIONAL_H
