#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

/**
 * Prompts the user for an integer input within a specified range.
 *
 * @param prompt The message to display to the user.
 * @param minVal The minimum allowed value.
 * @param maxVal The maximum allowed value.
 * @return The integer input by the user.
 */
int getIntegerInput(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value) || value < minVal || value > maxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between "
                 << minVal << " and " << maxVal << ".\n";
        } else {
            cin.ignore();
            return value;
        }
    }
}

/**
 * Prompts the user for a positive integer input.
 *
 * @param prompt The message to display to the user.
 * @return The positive integer input by the user.
 */
int getPositiveInteger(const string& prompt) {
    return getIntegerInput(prompt, 1, numeric_limits<int>::max());
}

/**
 * Prompts the user for a non-empty string input.
 *
 * @param prompt The message to display to the user.
 * @return The non-empty string input by the user.
 */
string getNonEmptyString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

/**
 * Prompts the user for a list of keywords.
 *
 * @return A vector containing the keywords entered by the user.
 */
vector<string> getKeywords() {
    vector<string> keywords;
    cout << "Enter keywords (separated by commas): ";
    string keywordInput;
    getline(cin, keywordInput);

    if (!keywordInput.empty()) {
        stringstream ss(keywordInput);
        string keyword;
        while (getline(ss, keyword, ',')) {
            keyword.erase(0, keyword.find_first_not_of(" \t"));
            keyword.erase(keyword.find_last_not_of(" \t") + 1);
            if (!keyword.empty()) {
                keywords.push_back(keyword);
            }
        }
    }
    return keywords;
}

/**
 * Checks if the given date is valid.
 * 
 * @param date The date to check.
 * @return True if the date is valid, false otherwise.
 */
bool checkValidDate(const string& date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
        return false;
    }
    
    // Check if all parts are numeric
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(date[i])) {
            return false;
        }
    }
    
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (day < 1 || month < 1 || month > 12 || year < 1900) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1];
}

#endif