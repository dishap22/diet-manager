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

#endif