#ifndef HOSPITALMANAGEMENT_INPUTHANDLER_H
#define HOSPITALMANAGEMENT_INPUTHANDLER_H

#include <iostream>
#include <limits>

using namespace std;

namespace handlers {

    void handleIntInput(int& input) {
        while (true) {
            if (!(cin >> input)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again." << endl;
            }
            else {
                string remainingInput;
                getline(cin, remainingInput);
                if (remainingInput.empty()) {
                    break;
                } else {
                    cout << "Invalid input. Please try again." << endl;
                }
            }
        }
    }


    bool isValidTimeFormat(const string& input) {
        if (input.length() != 5)
            return false;

        for (int i = 0; i < 5; ++i) {
            if (i == 2) {
                if (input[i] != ':')
                    return false;
            } else {
                if (!isdigit(input[i]))
                    return false;
            }
        }

        int hour = stoi(input.substr(0, 2));
        int minute = stoi(input.substr(3, 2));

        if (hour < 0 || hour > 23)
            return false;

        if (minute < 0 || minute > 59)
            return false;

        return true;
    }

    void handleTimeInput(string& input) {
        while (true) {
            getline(cin, input);
            if (!isValidTimeFormat(input)) {
                cout << "Invalid time format. Please enter time in the format hh:mm." << endl;
            } else {
                break;
            }
        }
    }
}

#endif //HOSPITALMANAGEMENT_INPUTHANDLER_H
