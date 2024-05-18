#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_HOSPITALMANAGEMENT_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_HOSPITALMANAGEMENT_H

#include <iostream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <fstream>
#include <limits>

#include "Doctor.h"
#include "Nurse.h"
#include "Specialist.h"
#include "Patient.h"
#include "Schedule.h"

using namespace std;

class HospitalManagement {
private:
    string professionalDataPath = "professional_data/";
    vector<Doctor> doctors;
    vector<Nurse> nurses;
    vector<Specialist> specialists;

    // Private constructor, making sure only one instance of HospitalManagement is created
    HospitalManagement() {
        loadProfessionalData();
    }

    // Destructor
    ~HospitalManagement() {
        saveProfessionalData();
    }

    // Utility function to get all professional data from filesystem
    void loadProfessionalData() {
        loadDoctors();
        loadNurses();
        loadSpecialists();
    }

    // Utility function to load doctors data from filesystem
    void loadDoctors() {
        for (const auto& filename : getAllProfessionalFiles(professionalDataPath + "doctors/")) {
            ifstream file(filename, ios::binary);
            if (file.is_open()) {
                Doctor doctor;
                doctor.deserialize(file);
                doctors.push_back(std::move(doctor));
                file.close();
            } else {
                cerr << "Error: Unable to open doctor data file " << filename << endl;
            }
        }
    }

    // Utility function to get all nurse data from filesystem
    void loadNurses() {
        for (const auto& filename : getAllProfessionalFiles(professionalDataPath + "nurses/")) {
            ifstream file(filename, ios::binary);
            if (file.is_open()) {
                Nurse nurse;
                nurse.deserialize(file);
                nurses.push_back(std::move(nurse));
                file.close();
            } else {
                cerr << "Error: Unable to open nurse data file " << filename << endl;
            }
        }
    }

    // Utility function to get all specialist data from filesystem
    void loadSpecialists() {
        for (const auto& filename : getAllProfessionalFiles(professionalDataPath + "specialists/")) {
            ifstream file(filename, ios::binary);
            if (file.is_open()) {
                Specialist specialist;
                specialist.deserialize(file);
                specialists.push_back(std::move(specialist));
                file.close();
            } else {
                cerr << "Error: Unable to open specialist data file " << filename << endl;
            }
        }
    }

    //Utility function to save all professional data
    void saveProfessionalData() {
        for (auto& doctor : doctors) {
            saveProfessionalData(doctor);
        }
        for (auto& nurse : nurses) {
            saveProfessionalData(nurse);
        }
        for (auto& specialist : specialists) {
            saveProfessionalData(specialist);
        }
    }

    // Utility function to save doctor data
    void saveProfessionalData(Doctor doctor) {
        ofstream file(professionalDataPath + "doctors/" + to_string(doctor.getId()) + ".dat", ios::binary);
        if (file.is_open()) {
            doctor.serialize(file);
            file.close();
        }
        else {
            cerr << "Error: Unable to save doctor data." << endl;
        }
    }

    // Utility function to save nurse data
    void saveProfessionalData(Nurse nurse) {
        ofstream file(professionalDataPath + "nurses/" + to_string(nurse.getId()) + ".dat", ios::binary);
        if (file.is_open()) {
            nurse.serialize(file);
            file.close();
        }
        else {
            cerr << "Error: Unable to save nurse data." << endl;
        }
    }

    // Utility function to save specialist data
    void saveProfessionalData(Specialist specialist) {
        ofstream file(professionalDataPath + "specialists/" + to_string(specialist.getId()) + ".dat", ios::binary);
        if (file.is_open()) {
            specialist.serialize(file);
            file.close();
        }
        else {
            cerr << "Error: Unable to save specialist data." << endl;
        }
    }

public:

    // Utility function to create an instance of HospitalManagement
    static HospitalManagement& create() {
        static HospitalManagement hospitalManagement;
        return hospitalManagement;
    }

    // Add doctor
    void addDoctor() {
        int id;
        string name;
        cout << "Enter ID: ";
        handlers::handleIntInput(id);
        cout << "Enter name: ";
        getline(cin,name);
        doctors.push_back(Doctor(id, name));
        saveProfessionalData(doctors.back());
    }

    // Add nurse
    void addNurse() {
        int id;
        string name;
        cout << "Enter ID: ";
        handlers::handleIntInput(id);
        cout << "Enter name: ";
        getline(cin,name);
        nurses.push_back(Nurse(id, name));
        saveProfessionalData(nurses.back());
    }

    // Add specialist
    void addSpecialist() {
        int id;
        string name, specialization;
        cout << "Enter ID: ";
        handlers::handleIntInput(id);
        cout << "Enter name: ";
        getline(cin,name);
        cout << "Enter specialization: ";
        getline(cin,specialization);
        specialists.push_back(Specialist(id, name, specialization));
        specialists.back().schedule = Schedule();
        saveProfessionalData(specialists.back());
    }

    // Add patient
    void addPatient() {
        int id;
        string name, time, specialization;
        char emergency;
        cout << "Enter ID: ";
        handlers::handleIntInput(id);
        cout << "Enter name: ";
        getline(cin,name);
        cout << "Enter required medical assistance branch: ";
        getline(cin,specialization);
        cout << "Enter time: ";
        handlers::handleTimeInput(time);
        cout << "Is patient an emergency? (y/n): ";
        cin >> emergency;
        if (emergency == 'y' || emergency == 'Y') {
            Patient patient(id, name, true, time);
            applyEmergencyProcedure(patient, specialization);
        }
        else {
            bool bookedCheck = false;
            Patient patient(id, name, false, time);
            cout << "Patient added" << endl;
            while (true) {
                for (Specialist& specialist : specialists) {
                    if (specialist.getSpecialization() == specialization) {
                        if (specialist.schedule.isAvailable(time)) {
                            specialist.schedule.bookTimeSlot(patient, time);
                            bookedCheck = true;
                            break;
                        }
                    }
                }
                if (!bookedCheck) {
                    time = Schedule::addMinutesToTime(time, 10);
                }
                else if (time == "17:00") {
                    cout << "No available specialists for this time" << endl;
                    break;
                }
                else {
                    break;
                }
            }
        }
    }

    // Emergency procedure function
    void applyEmergencyProcedure(Patient& patient, string& requiredSpecialization) {
        string bookTime = patient.getEntranceTime();
        while (true) {
            patient.setIsEmergency(true);
            bool bookedCheck = false;
            for (Specialist& specialist : specialists) {
                if (specialist.getSpecialization() == requiredSpecialization) {
                    if (specialist.schedule.isAvailable(bookTime)) {
                        specialist.schedule.bookTimeSlot(patient, bookTime);
                        bookedCheck = true;
                        cout << "Patient emergency procedure applied" << endl;
                        break;
                    }
                }
            }
            if (!bookedCheck) {
                for (Specialist& specialist : specialists) {
                    if (specialist.getSpecialization() == requiredSpecialization) {
                        specialist.schedule.moveTimeSlotsForward(bookTime);
                    }
                }
            }
            else {
                break;
            }
        }
    }

    // Get specialist by ID
    Specialist& getSpecialistById(int id) {
        for (Specialist& specialist : specialists) {
            if (specialist.getId() == id) {
                return specialist;
            }
        }
        cout << "Specialist not found" << endl;
        return specialists[0];
    }

    // Get doctor by ID
    Doctor& getDoctorById(int id) {
        for (Doctor& doctor : doctors) {
            if (doctor.getId() == id) {
                return doctor;
            }
        }
        cout << "Doctor not found" << endl;
        return doctors[0];
    }

    // Get nurse by ID
    Nurse& getNurseById(int id) {
        for (Nurse& nurse : nurses) {
            if (nurse.getId() == id) {
                return nurse;
            }
        }
        cout << "Nurse not found" << endl;
        return nurses[0];
    }

    // User management (kinda)
    int login(int choice) {
        int id;
        if (choice == 1) {
            cout << "Enter ID: ";
            handlers::handleIntInput(id);
            for (Specialist& specialist : specialists) {
                if (specialist.getId() == id) {
                    cout << "Welcome, " << specialist.getName() << endl;
                    return specialist.getId();
                }
            }
            cout << "Specialist not found" << endl;
            return 0;
        }
        else if (choice == 2) {
            cout << "Enter ID: ";
            handlers::handleIntInput(id);
            for (Doctor& doctor : doctors) {
                if (doctor.getId() == id) {
                    cout << "Welcome, " << doctor.getName() << endl;
                    return doctor.getId();
                }
            }
            cout << "Doctor not found" << endl;
            return 0;
        }
        else if (choice == 3) {
            cout << "Enter ID: ";
            handlers::handleIntInput(id);
            for (Nurse& nurse : nurses) {
                if (nurse.getId() == id) {
                    cout << "Welcome, " << nurse.getName() << endl;
                    return nurse.getId();
                }
            }
            cout << "Nurse not found" << endl;
            return 0;
        }
        else if (choice == 4) {
            int password = 1234;
            cout << "Enter password: ";
            handlers::handleIntInput(id);
            if (id == password) {
                cout << "Logged in as admin" << endl;
                return 5;
            }
            else {
                cout << "Invalid password" << endl;
                return 0;
            }
        }
        else {
            cout << "Invalid choice" << endl;
            return 0;
        }
    }

    void run() {
        while (true) {
            int choice, subchoice;
            cout << "1. Login as specialist" << endl;
            cout << "2. Login as doctor" << endl;
            cout << "3. Login as nurse" << endl;
            cout << "4. Login as admin" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter your choice: ";
            handlers::handleIntInput(subchoice);
            if (subchoice == 5) {
                break;
            }
            int id = login(subchoice);
            while (true) {
                if (subchoice == 1) {
                    if (id != 0) {
                        Specialist& specialist = getSpecialistById(id);
                        cout << "1. Check schedule" << endl;
                        cout << "2. Call patient" << endl;
                        cout << "3. Request assist" << endl;
                        cout << "4. Logout" << endl;
                        cout << "Enter your choice: ";
                        handlers::handleIntInput(choice);

                        if (choice == 1) {
                            specialist.schedule.print();
                        }

                        else if (choice == 2) {
                            string time;
                            bool bookedCheck = false;
                            cout << "Enter time: ";
                            handlers::handleTimeInput(time);
                            Patient patient = specialist.schedule.getPatientInTimeSlot(time);
                            if (patient.getId() != 0) {
                                specialist.work(patient);
                                specialist.schedule.clearTimeSlot(time);
                                while (true) {
                                    time = Schedule::addMinutesToTime(time, 10);
                                    for (Doctor& doctor : doctors) {
                                        if (doctor.schedule.isAvailable(time)) {
                                            doctor.schedule.bookTimeSlot(patient, time);
                                            cout << "Booked for: " << doctor.getName() << endl;
                                            bookedCheck = true;
                                            break;
                                        }
                                    }
                                    if (bookedCheck) {
                                        break;
                                    }
                                    else if (time == "23:50") {
                                        cout << "No doctors available" << endl;
                                        break;
                                    }
                                }
                            }
                            else {
                                cout << "Patient not found" << endl;
                            }
                        }

                        else if (choice == 3) {
                            string time;
                            cout << "Enter time: ";
                            handlers::handleTimeInput(time);
                            specialist.requestAssist(nurses, time);
                        }
                        else if (choice == 4) {
                            break;
                        }
                        else {
                            cout << "Invalid choice" << endl;
                        }
                    }
                    else {
                        break;
                    }
                }

                else if (subchoice == 2) {
                    if (id != 0) {
                        Doctor& doctor = getDoctorById(id);
                        cout << "1. Check schedule" << endl;
                        cout << "2. Call patient" << endl;
                        cout << "3. Logout" << endl;
                        cout << "Enter your choice: ";
                        handlers::handleIntInput(choice);
                        if (choice == 1) {
                            doctor.schedule.print();
                        }
                        else if (choice == 2) {
                            string time;
                            cout << "Enter time: ";
                            handlers::handleTimeInput(time);
                            Patient patient = doctor.schedule.getPatientInTimeSlot(time);
                            if (patient.getId() != 0) {
                                doctor.work(patient);
                                char letterChoice;
                                while (true) {
                                    cout << "a. Home medication" << endl;
                                    cout << "b. Personal medication" << endl;
                                    cout << "Enter your choice: ";
                                    cin >> letterChoice;
                                    if (letterChoice == 'a') {
                                        cout << "Patient discharged with medication:" << patient.getMedication() << endl;
                                        break;
                                    }
                                    else if (letterChoice == 'b') {
                                        bool bookCheck = false;
                                        while (true) {
                                            time = Schedule::addMinutesToTime(time, 10);
                                            for (Nurse& nurse : nurses) {
                                                if (nurse.schedule.isAvailable(time)) {
                                                    nurse.schedule.bookTimeSlot(patient, time);
                                                    cout << "Booked for: " << nurse.getName() << endl;
                                                    bookCheck = true;
                                                    break;
                                                }
                                            }
                                            if (bookCheck) {
                                                break;
                                            }
                                            else if (time == "23:50") {
                                                cout << "No nurse available" << endl;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    else {
                                        cout << "Invalid choice" << endl;
                                    }
                                }
                            }
                        }
                        else if (choice == 3) {
                            break;
                        }
                        else {
                            cout << "Invalid choice" << endl;
                        }
                    }
                    else {
                        break;
                    }
                }

                else if (subchoice == 3) {
                    if (id != 0) {
                        Nurse& nurse = getNurseById(id);
                        cout << "1. Check schedule" << endl;
                        cout << "2. Assist patient" << endl;
                        cout << "3. Logout" << endl;
                        cout << "Enter your choice: ";
                        handlers::handleIntInput(choice);
                        if (choice == 1) {
                            nurse.schedule.print();
                        }
                        else if (choice == 2) {
                            string time;
                            cout << "Enter time: ";
                            handlers::handleTimeInput(time);
                            Patient patient = nurse.schedule.getPatientInTimeSlot(time);
                            if (patient.getId() != 0) {
                                nurse.work(patient);
                            }
                        }

                        else if (choice == 3) {
                            break;
                        }
                        else {
                            cout << "Invalid choice" << endl;
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }

                else if (subchoice == 4) {
                    if (id == 5) {
                        cout << "1. Add specialist" << endl;
                        cout << "2. Add doctor" << endl;
                        cout << "3. Add nurse" << endl;
                        cout << "4. Add patient" << endl;
                        cout << "5. Logout" << endl;
                        cout << "Enter your choice: ";
                        handlers::handleIntInput(choice);
                        if (choice == 1) {
                            addSpecialist();
                        }
                        else if (choice == 2) {
                            addDoctor();
                        }
                        else if (choice == 3) {
                            addNurse();
                        }
                        else if (choice == 4) {
                            addPatient();
                        }
                        else if (choice == 5) {
                            break;
                        }
                        else {
                            cout << "Invalid choice" << endl;
                        }
                    }
                    else {
                        break;
                    }
                }

                else {
                    break;
                }
            }
        }
    }

    // Utility function to get all professional files in a directory
    static vector<string> getAllProfessionalFiles(const string& path) {
        vector<string> files;
        for (const auto& entry : filesystem::directory_iterator(path)) {
            files.push_back(entry.path().string());
        }
        return files;
    }
};

#endif //HOSPITAL_MANAGEMENT_SYSTEM_OOP_HOSPITALMANAGEMENT_H