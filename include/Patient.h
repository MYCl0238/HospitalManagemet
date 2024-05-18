#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_PATIENT_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_PATIENT_H

#include <string>
#include <fstream>
#include <utility>
#include <limits>

using namespace std;

struct Treatment {
    string name;
    string description;
    string treatment;
};

class Patient {
private:
    string entranceTime;
    string name;
    int id;
    Treatment treatment;
    string medication;
    bool isEmergency;

public:


    Patient() : id(0), name("NULL"), isEmergency(false), entranceTime("00:00"), medication("NULL"),
                treatment{"NULL", "NULL", "NULL"} {}

    Patient(int id, string  name, bool isEmergency, string  entranceTime)
            : id(id), name(std::move(name)), isEmergency(isEmergency), entranceTime(std::move(entranceTime)),
              medication("NULL"), treatment{"NULL", "NULL", "NULL"} {}

    // Accessor methods
    string getEntranceTime() const { return entranceTime; }
    string getName() const { return name; }
    int getId() const { return id; }
    bool getIsEmergency() const { return isEmergency; }
    Treatment getTreatment() const { return treatment; }
    string getMedication() const { return medication; }

    // Mutator methods
    void setIsEmergency(bool _isEmergency) { this->isEmergency = _isEmergency; }
    void setMedication(const string& _medication) { this->medication = _medication; }
    void setTreatment(string _name, string _description, string _treatment) {
        this->treatment.name = std::move(_name);
        this->treatment.description = std::move(_description);
        this->treatment.treatment = std::move(_treatment);
    }


    // Serialization
    void serialize(ofstream& ofs) const {
        ofs << id << '\n';
        ofs << entranceTime << '\n';
        ofs << name << '\n';
        ofs << treatment.name << '\n';
        ofs << treatment.description << '\n';
        ofs << treatment.treatment << '\n';
        ofs << medication << '\n';
        ofs << isEmergency << '\n';
    }

    // Deserialization
    void deserialize(ifstream& ifs) {
        ifs >> id;
        ifs.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(ifs, entranceTime);
        getline(ifs, name);
        getline(ifs, treatment.name);
        getline(ifs, treatment.description);
        getline(ifs, treatment.treatment);
        getline(ifs, medication);
        ifs >> isEmergency;
        ifs.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

#endif //HOSPITAL_MANAGEMENT_SYSTEM_OOP_PATIENT_H
