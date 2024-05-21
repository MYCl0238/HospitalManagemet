#ifndef HOSPITAL_MANAGEMENT_SYSTEM_OOP_SCHEDULE_H
#define HOSPITAL_MANAGEMENT_SYSTEM_OOP_SCHEDULE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include "Patient.h"

using namespace std;

struct TimeSlot {
    string start_time;
    string end_time;
    bool available;
    Patient patient;
};

class Schedule {
private:
    vector<TimeSlot> timeSlots;

public:
    Schedule() {
        // Initialize time slots for a day
        const int start_hour = 9;
        const int end_hour = 17;

        const int slot_duration = 10;

        for (int hour = start_hour; hour < end_hour; hour++) {
            for (int minute = 0; minute < 60; minute += slot_duration) {
                int adjusted_hour = hour + (minute + slot_duration) / 60;

                int end_minute = (minute + slot_duration) % 60;

                stringstream start_ss;
                start_ss << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
                string start_time = start_ss.str();

                stringstream end_ss;
                end_ss << setw(2) << setfill('0') << adjusted_hour << ":" << setw(2) << setfill('0') << end_minute;
                string end_time = end_ss.str();

                TimeSlot slot = {start_time, end_time, true, Patient()};
                timeSlots.push_back(slot);
            }
        }
    }

    // Add a time slot to the schedule
    void addTimeSlot(const TimeSlot& time_slot) {
        timeSlots.push_back(time_slot);
    }

    // Remove a time slot from the schedule
    void removeTimeSlot(string& time) {
        for (auto it = timeSlots.begin(); it != timeSlots.end(); ++it) {
            if (it->start_time == time) {
                timeSlots.erase(it);
                return;
            }
        }
        cout << "Time slot not found." << endl;
    }

    // Get the patient scheduled in a time slot
    Patient& getPatientInTimeSlot(const string& time) {
        for (auto& slot : timeSlots) {
            if (slot.start_time == time) {
                return slot.patient;
            }
        }
        cerr << "Time slot not found." << endl;
    }

    // Clear a time slot
    void clearTimeSlot(const string& time) {
        for (auto& slot : timeSlots) {
            if (slot.start_time == time) {
                clearTimeSlot(slot);
                return;
            }
        }
        cerr << "Time slot not found or already cleared." << endl;
    }

    bool isAvailable(const string& time) {
        for (const auto& slot : timeSlots) {
            if (slot.start_time == time && (slot.available || slot.patient.getId() == 0)) {
                return true;
            }
        }
        return false;
    }


    // Book a time slot for a patient
    void bookTimeSlot(Patient& patient, const string& time) {
        for (auto& slot : timeSlots) {
            if (slot.start_time == time && (slot.available || slot.patient.getId() == 0)) {
                slot.available = false;
                slot.patient = patient;
                cout << "Time slot booked." << endl;
                return;
            }
        }
        cout << "Time slot not found or already booked." << endl;
    }

    // Book a time slot for a personal
    void bookTimeSlot(const string& time) {
        for (auto& slot : timeSlots) {
            if (slot.start_time == time && (slot.available || slot.patient.getId() == 0)) {
                slot.available = false;
                cout << "Time slot booked." << endl;
                return;
            }
        }
        cout << "Time slot not found or already booked." << endl;
    }

    // Move time slots forward by 10 minutes starting from a given time
    void moveTimeSlotsForward(const string& startTime) {
        int startMinutes = convertToMinutes(startTime);
        int iterator = 0;
        bool foundInsertionPoint = false;
        for (auto & timeSlot : timeSlots) {
            int slotStartMinutes = convertToMinutes(timeSlot.start_time);
            if (slotStartMinutes >= startMinutes) {
                continue;
            }
            else {
                if (!foundInsertionPoint) {
                    iterator++;
                }
            }
            if (!foundInsertionPoint && slotStartMinutes > startMinutes) {
                foundInsertionPoint = true;
            }
        }
        timeSlots.insert(timeSlots.begin() + iterator, TimeSlot{startTime, addMinutesToTime(startTime, 10), true, Patient()});
    }

    // Print the schedule
    void print() {
        for (const auto& slot : timeSlots) {
            cout << slot.start_time << " - " << slot.end_time << " - " <<
                 (slot.patient.getId() == 0 ? "Available" : (slot.available ? "Available" : "Booked"))
                 << endl;
        }
    }

    // Serialization method to save schedule data to a file
    void serialize(ofstream& ofs) {
        ofs << timeSlots.size() << endl;
        for (auto& slot : timeSlots) {
            ofs << slot.start_time << endl;
            ofs << slot.end_time << endl;
            ofs << slot.available << endl;
            slot.patient.serialize(ofs);
            if (slot.patient.getId() != 0) {
                savePatientData(slot.patient);
            }
        }
    }

    // Deserialization method to load schedule data from a file
    void deserialize(ifstream& ifs) {
        int numTimeSlots;
        ifs >> numTimeSlots;
        timeSlots.clear();
        for (int i = 0; i < numTimeSlots; i++) {
            TimeSlot slot;
            ifs >> slot.start_time;
            ifs >> slot.end_time;
            ifs >> slot.available;
            slot.patient.deserialize(ifs);
            timeSlots.push_back(slot);
        }
    }

    // Utility function to add minutes to a given time
    static string addMinutesToTime(const string& timeString, int minutesToAdd) {
        int hours = stoi(timeString.substr(0, 2));
        int minutes = stoi(timeString.substr(3, 2));
        int totalMinutes = (hours * 60) + minutes;
        totalMinutes += minutesToAdd;
        hours = totalMinutes / 60;
        minutes = totalMinutes % 60;
        if (hours < 10) {
            if (minutes < 10) {
                return "0" + to_string(hours) + ":0" + to_string(minutes);
            }
            else {
                return "0" + to_string(hours) + ":" + to_string(minutes);
            }
        }
        else {
            if (minutes < 10) {
                return to_string(hours) + ":0" + to_string(minutes);
            }
            else {
                return to_string(hours) + ":" + to_string(minutes);
            }
        }
    }

    static void savePatientData(Patient &patient) {
        ofstream file("patient_data/" + to_string(patient.getId()) + ".dat", ios::binary);
        if (file.is_open()) {
            patient.serialize(file);
            file.close();
        } else {
            cerr << "Error: Unable to save patient data." << endl;
        }
    }

    // Utility function to convert time to minutes
    static int convertToMinutes(const string& timeString) {
        int hours = stoi(timeString.substr(0, 2));
        int minutes = stoi(timeString.substr(3, 2));
        return hours * 60 + minutes;
    }

    // Utility function to clear a time slot
    static void clearTimeSlot(TimeSlot &slot) {
        slot.available = true;
        slot.patient = Patient();
    }

};

#endif //HOSPITAL_MANAGEMENT_SYSTEM_OOP_SCHEDULE_H
