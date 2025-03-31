#ifndef USERPROFILE_H
#define USERPROFILE_H

#include "Utils.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
using namespace std;

/**
 * Represents a daily record of user stats.
 */
struct DailyRecord {
    int age;
    int height;
    int weight;
    string gender;
    string activityLevel;
};

/**
 * Represents a user with age, height, weight, gender and activity level.
 */
class UserProfile {
private:
    map<string, DailyRecord> dailyRecords;
    string caloryCalculationMethod = "Harris-Benedict";

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
     * Calculate Harris-Benedict BMR (Basal Metabolic Rate)
     * 
     * @param record The daily record to use for calculation
     * @return The calculated target calories
     */
    int calculateHarrisBenedictBMR(const DailyRecord& record) {
        double bmr = 0;
        
        if (record.gender == "Male") {
            bmr = 88.362 + (13.397 * record.weight) + (4.799 * record.height) - (5.677 * record.age);
        } else { // Female
            bmr = 447.593 + (9.247 * record.weight) + (3.098 * record.height) - (4.330 * record.age);
        }
        
        // Activity multiplier
        double activityMultiplier = 1.2; // Sedentary
        if (record.activityLevel == "Light") {
            activityMultiplier = 1.375;
        } else if (record.activityLevel == "Moderate") {
            activityMultiplier = 1.55;
        } else if (record.activityLevel == "Active") {
            activityMultiplier = 1.725;
        } else if (record.activityLevel == "Very Active") {
            activityMultiplier = 1.9;
        }
        
        return static_cast<int>(bmr * activityMultiplier);
    }

    /**
     * Calculate Mifflin-St Jeor formula
     * 
     * @param record The daily record to use for calculation
     * @return The calculated target calories
     */
    int calculateMifflinStJeor(const DailyRecord& record) {
        double bmr = 0;
        
        if (record.gender == "Male") {
            bmr = (10 * record.weight) + (6.25 * record.height) - (5 * record.age) + 5;
        } else { // Female
            bmr = (10 * record.weight) + (6.25 * record.height) - (5 * record.age) - 161;
        }
        
        // Activity multiplier
        double activityMultiplier = 1.2; // Sedentary
        if (record.activityLevel == "Light") {
            activityMultiplier = 1.375;
        } else if (record.activityLevel == "Moderate") {
            activityMultiplier = 1.55;
        } else if (record.activityLevel == "Active") {
            activityMultiplier = 1.725;
        } else if (record.activityLevel == "Very Active") {
            activityMultiplier = 1.9;
        }
        
        return static_cast<int>(bmr * activityMultiplier);
    }

    /**
     * Calculate Katch-McArdle formula (requires body fat percentage)
     * Using a default body fat estimation for simplicity
     * 
     * @param record The daily record to use for calculation
     * @return The calculated target calories
     */
    int calculateKatchMcArdle(const DailyRecord& record) {
        // Estimate body fat percentage based on BMI
        double bmi = record.weight / ((record.height / 100.0) * (record.height / 100.0));
        double bodyFatPercentage = 0;
        
        if (record.gender == "Male") {
            bodyFatPercentage = 1.20 * bmi + 0.23 * record.age - 16.2;
        } else {
            bodyFatPercentage = 1.20 * bmi + 0.23 * record.age - 5.4;
        }
        
        // Cap body fat percentage to reasonable limits
        if (bodyFatPercentage < 5) bodyFatPercentage = 5;
        if (bodyFatPercentage > 45) bodyFatPercentage = 45;
        
        // Calculate lean body mass
        double lbm = record.weight * (1 - (bodyFatPercentage / 100));
        
        // Katch-McArdle formula for BMR based on lean body mass
        double bmr = 370 + (21.6 * lbm);
        
        // Activity multiplier
        double activityMultiplier = 1.2; // Sedentary
        if (record.activityLevel == "Light") {
            activityMultiplier = 1.375;
        } else if (record.activityLevel == "Moderate") {
            activityMultiplier = 1.55;
        } else if (record.activityLevel == "Active") {
            activityMultiplier = 1.725;
        } else if (record.activityLevel == "Very Active") {
            activityMultiplier = 1.9;
        }
        
        return static_cast<int>(bmr * activityMultiplier);
    }

    DailyRecord getLastRecord() {
        if (dailyRecords.empty()) {
            return {25, 175, 70, "Male", "Moderate"};
        }
        return dailyRecords.rbegin()->second;
    }
public:
    /**
     * Construct a UserProfile with existing values using the last record
     */
    UserProfile() {
        loadRecords();
    }

    /**
     * Constructs a UserProfile object with the given gender, age, height, weight, and activity level.
     *
     * @param g The gender of the user
     * @param a The age of the user
     * @param h The height of the user
     * @param w The weight of the user
     * @param activity The activity level of the user
     */
    UserProfile(int a, int h, int w, const string& g, const string& activity) {
        string today = getTodayDate();
        DailyRecord record = {a, h, w, g, activity}; // Include gender in the record
        dailyRecords[today] = record;
    }

    /**
     * Loads user records from file
     */

    void loadRecords() {
        ifstream file("user_profile.txt");
        if (!file) {
            return;
        }
    
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string date, gender, activityLevel;
            DailyRecord record;
    
            getline(ss, date, '|');
            ss >> record.age;
            ss.ignore();
            ss >> record.height;
            ss.ignore();
            ss >> record.weight;
            ss.ignore();
            getline(ss, gender, '|'); // Read gender
            getline(ss, activityLevel, '|'); // Read activity level
            record.gender = gender;
            record.activityLevel = activityLevel;
    
            if (checkValidDate(date)) {
                dailyRecords[date] = record;
            }
        }
    
        cout << "Profile records loaded successfully.\n";
        file.close();
    }

    /**
     * Save user records to file
     */
    void saveRecords() {
        ofstream file("user_profile.txt");
        if (!file) {
            cout << "Error saving profile records!\n";
            return;
        }
    
        for (const auto& day : dailyRecords) {
            file << day.first << "|" 
                 << day.second.age << "|" 
                 << day.second.height << "|" 
                 << day.second.weight << "|" 
                 << day.second.gender << "|" 
                 << day.second.activityLevel << "\n";
        }
    
        cout << "Profile records saved successfully.\n";
        file.close();
    }

    /**
     * Displays the current user's profile.
     */
    void displayProfile() {
        // Get last record or default record if there are no records
        DailyRecord record = getLastRecord();
        cout << "Age: " << record.age << endl;
        cout << "Height: " << record.height << " cm\n";
        cout << "Weight: " << record.weight << " kg\n";
        cout << "Activity Level: " << record.activityLevel << endl;
        cout << "Calorie Calculation Method: " << caloryCalculationMethod << endl;
    }

    /**
     * Updates the user's profile with new data.
     */
    void updateProfile() {
        string today = getTodayDate();
        DailyRecord lastRecord = getLastRecord();
        int age, weight;
        age = getIntegerInput("Enter your age: ", 1, 150);
        weight = getIntegerInput("Enter your weight (kg): ", 1, 200);
        lastRecord.age = age;
        lastRecord.weight = weight;
        dailyRecords[today] = lastRecord;
    }

    void updateCaloreCalculationMethod() {
        cout << "Select a method for calculating target calories:\n"
             << "(1) Harris-Benedict\n"
             << "(2) Mifflin-St Jeor\n"
             << "(3) Katch-McArdle\n";
        int option = getIntegerInput("Enter your choice: ", 1, 3);
        switch (option) {
            case 1:
                caloryCalculationMethod = "Harris-Benedict";
                break;
            case 2:
                caloryCalculationMethod = "Mifflin-St Jeor";
                break;
            case 3:
                caloryCalculationMethod = "Katch-McArdle";
                break;
        }
    }

    /**
     * Updates the user's activity level.
     */
    void updateActivityLevel() {
        string today = getTodayDate();
        DailyRecord lastRecord = getLastRecord();
        string activityLevel;
        cout << "Select your activity level:\n"
             << "(1) Sedentary\n"
             << "(2) Light\n"
             << "(3) Moderate\n"
             << "(4) Active\n"
             << "(5) Very Active\n";
        int option = getIntegerInput("Enter your choice: ", 1, 5);
        switch (option) {
            case 1:
                activityLevel = "Sedentary";
                break;
            case 2:
                activityLevel = "Light";
                break;
            case 3:
                activityLevel = "Moderate";
                break;
            case 4:
                activityLevel = "Active";
                break;
            case 5:
                activityLevel = "Very Active";
                break;
        }
        lastRecord.activityLevel = activityLevel;
        dailyRecords[today] = lastRecord;
    }

    /**
     * Updates the user's calorie calculation method.
     */
    void updateCalorieCalculation() {
        cout << "Select a method for calculating target calories:\n"
             << "(1) Harris-Benedict\n"
             << "(2) Mifflin-St Jeor\n"
             << "(3) Katch-McArdle\n";
        int option = getIntegerInput("Enter your choice: ", 1, 3);
        switch (option) {
            case 1:
                caloryCalculationMethod = "Harris-Benedict";
                break;
            case 2:
                caloryCalculationMethod = "Mifflin-St Jeor";
                break;
            case 3:
                caloryCalculationMethod = "Katch-McArdle";
                break;
        }
    }

    /**
     * Gets the user's target calories for the day.
     * @param date The date to get the target calories for.
     * @return The target calories for the day.
     */
    int getTargetCalories(const string& date) {
        // Get first occuring record dated on or before the given date, or the first record if none found, or the default record if none found
        DailyRecord record = getLastRecord();
        auto found = dailyRecords.lower_bound(date);
        if (found != dailyRecords.end() && found->first == date) {
            record = found->second;
        } else if (found != dailyRecords.begin()) {
            record = prev(found)->second;
        }
        if (caloryCalculationMethod == "Harris-Benedict") {
            return calculateHarrisBenedictBMR(record);
        } else if (caloryCalculationMethod == "Mifflin-St Jeor") {
            return calculateMifflinStJeor(record);
        } else if (caloryCalculationMethod == "Katch-McArdle") {
            return calculateKatchMcArdle(record);
        }
        return -1;
    }
};

#endif