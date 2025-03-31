#include "UserProfile.h"
#include "FoodDatabase.h"
#include "DailyLog.h"
#include <iostream>
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
 * Creates a composite food item from existing food items.
 *
 * @param database The food database to search for the food items and add the food to.
 */
void createCompositeFood(FoodDatabase& database) {
    string compositeName = getNonEmptyString("Enter name for the new composite food: ");

    cout << "Enter optional keywords (leave empty to generate from ingredients):\n";
    vector<string> keywords = getKeywords();

    database.displayAllFoods();
    vector<CompositeFood::Ingredient> ingredients;

    while (true) {
        int index = getIntegerInput("Food index (-1 to finish): ", -1, database.foods.size() - 1);
        if (index == -1) {
            if (ingredients.empty()) {
                cout << "You must add at least one ingredient.\n";
                continue;
            }
            break;
        }

        bool duplicate = false;
        for (const auto& ing : ingredients) {
            if (ing.food == database.foods[index]) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "This ingredient is already added. Try again.\n";
            continue;
        }

        int servings = getPositiveInteger("Number of servings: ");
        ingredients.push_back({database.foods[index], servings});
        cout << "Added " << servings << " serving(s) of " << database.foods[index]->name << ".\n";
    }

    CompositeFood* newComposite = new CompositeFood(compositeName, ingredients, keywords);
    database.addCompositeFood(newComposite);
    cout << "Composite food created: " << compositeName
         << " with " << newComposite->calories << " calories.\n";
}

/**
 * Adds a new basic food item to the database.
 *
 * @param database The food database to add the food item to.
 */
void addBasicFood(FoodDatabase& database) {
    string name = getNonEmptyString("Enter food name: ");
    vector<string> keywords = getKeywords();

    int calories;
    while (true) {
        cout << "Enter calories: ";
        if (cin >> calories && calories >= 0) {
            cin.ignore();
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a non-negative number.\n";
    }

    database.addFood(new Food(name, keywords, calories));
    cout << "New basic food added: " << name << endl;
}

/**
 * Displays the options for logging food items.
 *
 * @param database The food database to search for food items.
 * @param log The daily log to add food items to.
 * @param user The user profile to calculate calorie intake.
 */
void logOptions(FoodDatabase& database, DailyLog& log, UserProfile& user) {
    cout << "\nChoose an option:\n"
         << "(1) Save Log\n"
         << "(2) Add Log Entry\n"
         << "(3) Delete Log Entry\n"
         << "(4) Undo Log Entry\n"
         << "(5) View Log\n"
         << "(6) View Log by date\n";
    
    try {
        switch (getIntegerInput("Enter your choice: ", 1, 6)) {
            case 1:
                log.saveLog("daily_log.txt");
                break;
            case 2:
                log.logFood(database);
                break;
            case 3:
                log.removeFood();
                break;
            case 4:
                log.undoLog();
                break;
            case 5:
                log.displayAllLogs(database);
                break;
            case 6:
                log.displayLogByDate(database);
                break;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

/**
 * The main function of the program.
 */
int main() {
    UserProfile user("DummyUser", "Male", 25, 175, 70, "Moderate");
    FoodDatabase database;
    DailyLog log;

    database.loadDatabase("food_database.txt");

    while (true) {
        cout << "\nChoose an option:\n"
             << "(1) Log food\n"
             << "(2) Create composite food\n"
             << "(3) View all foods\n"
             << "(4) Add new basic food\n"
             << "(5) Save database\n"
             << "(6) Exit\n";

        int option = getIntegerInput("Enter your choice: ", 1, 6);

        try {
            switch (option) {
                case 1:
                    logOptions(database, log, user);
                    break;
                case 2:
                    createCompositeFood(database);
                    break;
                case 3:
                    database.displayAllFoods();
                    break;
                case 4:
                    addBasicFood(database);
                    break;
                case 5:
                    database.saveDatabase("food_database.txt");
                    break;
                case 6:
                    database.saveDatabase("food_database.txt");
                    log.saveLog("daily_log.txt");
                    log.displayAllLogs(database);
                    cout << "Exiting program.\n";
                    return 0;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}