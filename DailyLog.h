#ifndef DAILYLOG_H
#define DAILYLOG_H

#include "Utils.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

class FoodDatabase;
class Food;
class UserProfile;

/**
 * Represents a daily log of food items consumed.
 */
class DailyLog {
private:
    map<string, unordered_map<string, int>> log;
    stack<pair<string, pair<string, int>>> undoStack;

    /**
     * Gets today's date formatted as DD/MM/YYYY.
     * 
     * @return Today's date as a string.
     */
    string getTodayDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        string day = to_string(ltm->tm_mday);
        if (day.length() == 1) day = "0" + day;
        
        string month = to_string(1 + ltm->tm_mon);
        if (month.length() == 1) month = "0" + month;
        
        string year = to_string(1900 + ltm->tm_year);
        
        return day + "/" + month + "/" + year;
    }

    /**
     * Processes the date input by the user.
     *
     * @param date The date input by the user.
     * @return True if the date is valid, false otherwise.
     */
    bool processDate(string& date) {
        if (date.empty()) {
            date = getTodayDate();
            cout << "Using today's date: " << date << endl;
            return true;
        } else {
            if (!checkValidDate(date)) {
                cout << "Invalid date format. Please enter a valid date in the format of DD/MM/YYYY.\n";
                return false;
            }
        }
        return true;
    }

    /**
     * Gets a valid integer input from the user.
     * 
     * @param prompt The prompt to display.
     * @param min The minimum valid value.
     * @return The valid integer input, or -1 if the input was invalid.
     */
    int getIntInput(const string& prompt, int min = 0) {
        string input;
        int value;
        
        cout << prompt;
        getline(cin, input);
        
        // Check for empty input
        if (input.empty()) {
            return -1;
        }
        
        // Check if input contains only digits
        if (!all_of(input.begin(), input.end(), ::isdigit)) {
            cout << "Invalid input. Please enter a positive number.\n";
            return -1;
        }
        
        try {
            value = stoi(input);
            if (value < min) {
                cout << "Invalid input. Please enter a number greater than or equal to " << min << ".\n";
                return -1;
            }
        } catch (const exception& e) {
            cout << "Invalid input. Please enter a valid number.\n";
            return -1;
        }
        
        return value;
    }

    /**
     * Normalize search string (lowercase, trim whitespace)
     */
    string normalizeString(const string& input) {
        string result = input;
        // Convert to lowercase
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        // Trim leading whitespace
        result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
        // Trim trailing whitespace
        result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1);
        return result;
    }

    /**
     * Displays a food selection menu for the user
     * 
     * @param foundFoods List of foods to display
     * @return Selected food or nullptr if canceled
     */
    Food* displayFoodSelectionMenu(const vector<Food*>& foundFoods) {
        if (foundFoods.empty()) {
            return nullptr;
        }
        
        cout << "Found " << foundFoods.size() << " foods:\n";
        
        for (size_t i = 0; i < foundFoods.size(); i++) {
            cout << i + 1 << ". " << foundFoods[i]->name << " - " 
                 << foundFoods[i]->calories << " calories per serving" << endl;
        }
        
        int choice = getIntInput("\nEnter the number of your choice (or 0 to cancel): ", 0);
        if (choice == 0 || choice == -1 || choice > static_cast<int>(foundFoods.size())) {
            return nullptr;
        }
        
        return foundFoods[choice - 1];
    }

public:
    /**
     * Loads the log from a file.
     */
    DailyLog() {
        ifstream file("daily_log.txt");
        if (!file) {
            cout << "No existing log found. Starting with empty log.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string date;
            getline(ss, date, '|');
            
            string entry;
            while (getline(ss, entry, ';')) {
                stringstream entrySS(entry);
                string foodName;
                int servings;
                getline(entrySS, foodName, ',');
                entrySS >> servings;
                log[date][foodName] = servings;
            }
        }

        cout << "Log loaded successfully.\n";
        file.close();
    }

    /**
     * Saves the log to a file.
     *
     * @param filename The name of the file to save the log to.
     */
    void saveLog(const string& filename = "daily_log.txt") {
        ofstream file(filename);
        if (!file) {
            cout << "Error saving log!\n";
            return;
        }

        for (auto& day : log) {
            // Must be in the format of date (DD/MM/YYYY)|food1,servings1;food2,servings2;...
            file << day.first << "|";
            for (auto& entry : day.second) {
                file << entry.first << "," << entry.second << ";";
            }
            file << endl;
        }

        cout << "Log saved successfully.\n";
        file.close();
    }

    /**
     * Logs a food item to the log.
     *
     * @param database The food database to use.
     */
    void logFood(FoodDatabase& database) {
        string date;
        cout << "Enter date (DD/MM/YYYY or press Enter for today): ";
        getline(cin, date);
        if (!processDate(date)) {
            return;
        }
        
        // Ask user if they want to browse all foods or search by keywords
        cout << "1. Browse all foods\n";
        cout << "2. Search by keywords\n";
        int option = getIntInput("Enter your choice: ", 1);
        
        if (option == -1) {
            cout << "Invalid option. Logging canceled.\n";
            return;
        }
        
        vector<Food*> foundFoods;
        
        if (option == 1) {
            // Show all foods
            database.displayAllFoods();
            
            cout << "Enter a food name to log (or press Enter to cancel): ";
            string foodName;
            getline(cin, foodName);
            if (foodName.empty()) {
                cout << "Logging canceled.\n";
                return;
            }
            
            // Try exact match first
            Food* selectedFood = database.searchOneFood(foodName);
            if (!selectedFood) {
                cout << "Food not found. Logging canceled.\n";
                return;
            }
            
            // Get servings
            int servings = getIntInput("Enter the number of servings: ", 1);
            if (servings == -1) {
                cout << "Invalid servings. Logging canceled.\n";
                return;
            }
            
            log[date][selectedFood->name] += servings;
            undoStack.push(make_pair(date, make_pair(selectedFood->name, servings)));
            cout << "Logged " << servings << " serving(s) of " << selectedFood->name << ".\n";
            
        } else if (option == 2) {
            // Search by keywords
            vector<string> keywords;
            string input;
            
            cout << "Enter search keywords (separate with spaces): ";
            getline(cin, input);
            
            if (input.empty()) {
                cout << "No keywords entered. Logging canceled.\n";
                return;
            }
            
            // Split input into keywords
            stringstream ss(input);
            string keyword;
            while (ss >> keyword) {
                keywords.push_back(keyword);
            }
            
            // Ask for search mode
            cout << "1. Match ANY keyword\n";
            cout << "2. Match ALL keywords\n";
            int searchMode = getIntInput("Enter search mode: ", 1);
            
            if (searchMode == -1 || searchMode > 2) {
                cout << "Invalid search mode. Logging canceled.\n";
                return;
            }

            // Search foods
            foundFoods = database.searchFood(keywords, searchMode - 1);
            
            if (foundFoods.empty()) {
                cout << "No foods found with the given keywords.\n";
                return;
            }
            
            // Display selection menu
            Food* selectedFood = displayFoodSelectionMenu(foundFoods);
            if (!selectedFood) {
                cout << "Logging canceled.\n";
                return;
            }
            
            // Get servings
            int servings = getIntInput("Enter the number of servings: ", 1);
            if (servings == -1) {
                cout << "Invalid servings. Logging canceled.\n";
                return;
            }
            
            log[date][selectedFood->name] += servings;
            undoStack.push(make_pair(date, make_pair(selectedFood->name, servings)));
            cout << "Logged " << servings << " serving(s) of " << selectedFood->name << ".\n";
        }
    }

    /**
     * Removes a food from the log.
     */
    void removeFood() {
        string date;
        cout << "Enter date (DD/MM/YYYY or press Enter for today): ";
        getline(cin, date);
        if (!processDate(date)) {
            return;
        }

        // Show all log entries for the given date
        auto found = log.find(date);
        if (found == log.end() || found->second.empty()) {
            cout << "No log entries found for " << date << ".\n";
            return;
        }
        
        // Display all logs of the given date with numbers for selection
        cout << "\nFood log for " << date << ":\n";
        
        vector<string> foodNames;
        int i = 1;
        for (auto& entry : found->second) {
            cout << i << ". " << entry.first << " - " << entry.second << " serving(s)" << endl;
            foodNames.push_back(entry.first);
            i++;
        }

        int choice = getIntInput("\nEnter the number of the food to remove (or 0 to cancel): ", 0);
        if (choice == 0 || choice == -1 || choice > static_cast<int>(foodNames.size())) {
            cout << "Removal canceled.\n";
            return;
        }
        
        string foodToRemove = foodNames[choice - 1];
        int currentServings = found->second[foodToRemove];
        
        // Remove the entire entry
        undoStack.push(make_pair(date, make_pair(foodToRemove, -currentServings)));
        found->second.erase(foodToRemove);
        cout << "Removed '" << foodToRemove << "' from the log.\n";
        
        // Clean up empty dates
        if (found->second.empty()) {
            log.erase(date);
        }
    }

    /**
     * Undoes the last log entry.
     */
    void undoLog() {
        if (undoStack.empty()) {
            cout << "No log entries to undo.\n";
            return;
        }

        auto entry = undoStack.top();
        undoStack.pop();
        
        string date = entry.first;
        string food = entry.second.first;
        int servings = entry.second.second;
        
        // If the date doesn't exist in the log, it was completely removed in a previous operation
        if (log.find(date) == log.end() && servings > 0) {
            log[date][food] = servings;
            cout << "Undid the last log entry: Added back " << servings << " serving(s) of '" << food << "' on " << date << ".\n";
            return;
        }
        
        // Normal case: modify the existing entry
        log[date][food] -= servings;
        
        if (log[date][food] <= 0) {
            log[date].erase(food);
            cout << "Undid the last log entry: Removed '" << food << "' from " << date << ".\n";
        } else {
            cout << "Undid the last log entry: Changed '" << food << "' to " << log[date][food] << " serving(s) on " << date << ".\n";
        }
        
        // Clean up empty dates
        if (log[date].empty()) {
            log.erase(date);
        }
    }

    /**
     * Displays the log for a specific date.
     */
    void displayLogByDate(FoodDatabase& database, UserProfile& user){
        string date;
        cout << "Enter date (DD/MM/YYYY or press Enter for today): ";
        getline(cin, date);
        if (!processDate(date)) {
            return;
        }
    
        auto found = log.find(date);
        if (found == log.end() || found->second.empty()) {
            cout << "No log entries found for " << date << ".\n";
            return;
        }
    
        cout << "\nFood log for " << date << ":\n";
    
        int i = 1;
        int totalCalories = 0; // Track total calories
        for (auto& entry : found->second) {
            Food* food = database.searchOneFood(entry.first);
            if (food) {
                int calories = food->calories * entry.second;
                totalCalories += calories;
                cout << i << ". " << entry.first << " - " << entry.second << " serving(s) (" << calories << " calories)\n";
            } else {
                cout << i << ". " << entry.first << " - " << entry.second << " serving(s) (calories unknown)\n";
            }
            i++;
        }
    
        cout << "Total calories consumed: " << totalCalories << " calories\n";
        cout << "Target calories for the day: " << user.getTargetCalories(date) << " calories\n";
        cout << "Calorie excess: " << - user.getTargetCalories(date) + totalCalories << " calories\n";
    }

    /**
     * Displays the complete log, including a summary of total calories consumed for each day.
     */
    void displayAllLogs(FoodDatabase& database, UserProfile& user) {
        if (log.empty()) {
            cout << "No log entries found.\n";
            return;
        }

        cout << "\nComplete food log:\n";
        for (auto& day : log) {
            if (day.second.empty()) {
                continue;
            }

            cout << "\nDate: " << day.first << "\n";

            int i = 1;
            int totalCalories = 0; // Track total calories for the day
            for (auto& entry : day.second) {
                Food* food = database.searchOneFood(entry.first);
                if (food) {
                    int calories = food->calories * entry.second;
                    totalCalories += calories;
                    cout << i << ". " << entry.first << " - " << entry.second << " serving(s) (" << calories << " calories)\n";
                } else {
                    cout << i << ". " << entry.first << " - " << entry.second << " serving(s) (calories unknown)\n";
                }
                i++;
            }

            cout << "Total calories consumed for " << day.first << ": " << totalCalories << " calories\n";
            cout << "Target calories for the day: " << user.getTargetCalories(day.first) << " calories\n";
            cout << "Calorie excess: " << - user.getTargetCalories(day.first) + totalCalories << " calories\n";
        }
    }
};

#endif