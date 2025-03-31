#include "UserProfile.h"
#include "FoodDatabase.h"
#include "DailyLog.h"
#include "Utils.h"
#include <iostream>

using namespace std;

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
 * Displays the Update Profile submenu and handles user choices.
 * 
 * @param user The user profile to update.
 * @param log The daily log to update with new profile information.
 * @param database The food database to recalculate calorie intake.
 */

 void updateProfileMenu(UserProfile& user, DailyLog& log, FoodDatabase& database) {
    while (true) {
        cout << "\nUpdate Profile Menu:\n"
             << "(1) View Profile Information\n"
             << "(2) Update Profile Information\n"
             << "(3) Update Calorie Calculation Method\n"
             << "(4) Update Activity Level\n"
             << "(5) Save Profile and Return to Main Menu\n";

        int option = getIntegerInput("Enter your choice: ", 1, 5);

        try {
            switch (option) {
                case 1:
                    user.displayProfile();
                    break;
                case 2:
                    user.updateProfile();
                    break;
                case 3:
                    user.updateCalorieCalculation();
                    break;
                case 4:
                    user.updateActivityLevel();
                    break;
                case 5:
                    user.saveRecords();
                    return;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
 }

/**
 * Displays the Log Foods submenu and handles user choices.
 *
 * @param database The food database to search for food items.
 * @param log The daily log to add food items to.
 * @param user The user profile to calculate calorie intake.
 */
void logFoodsMenu(FoodDatabase& database, DailyLog& log, UserProfile& user) {
    while (true) {
        cout << "\nLog Foods Menu:\n"
             << "(1) Save Log\n"
             << "(2) Add Log Entry\n"
             << "(3) Delete Log Entry\n"
             << "(4) Undo Log Entry\n"
             << "(5) View Log\n"
             << "(6) View Log by Date\n"
             << "(7) Return to Main Menu\n";

        int option = getIntegerInput("Enter your choice: ", 1, 7);

        try {
            switch (option) {
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
                    log.displayAllLogs(database, user);
                    break;
                case 6:
                    log.displayLogByDate(database, user);
                    break;
                case 7:
                    return; // Exit the Log Foods menu
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

/**
 * Displays the Manage Foods submenu and handles user choices.
 *
 * @param database The food database to manage.
 */
void manageFoodsMenu(FoodDatabase& database) {
    while (true) {
        cout << "\nManage Foods Menu:\n"
             << "(1) Create Composite Food\n"
             << "(2) View All Foods\n"
             << "(3) Add New Basic Food\n"
             << "(4) Save Database\n"
             << "(5) Return to Main Menu\n";

        int option = getIntegerInput("Enter your choice: ", 1, 5);

        try {
            switch (option) {
                case 1:
                    createCompositeFood(database);
                    break;
                case 2:
                    database.displayAllFoods();
                    break;
                case 3:
                    addBasicFood(database);
                    break;
                case 4:
                    database.saveDatabase("food_database.txt");
                    break;
                case 5:
                    return; // Exit the Manage Foods menu
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

/**
 * The main function of the program.
 */
int main() {
    int age, weight, height;
    string gender, activity;
    ifstream file("user_profile.txt");

    UserProfile user;
    // Check whether user_profile.txt exists and if it doesn't, prompt the user to create a new profile
    if(!file) {
        cout << "No existing profile found. Please create a new profile.\n";
        age = getIntegerInput("Enter your age: ", 1, 150);
        height = getIntegerInput("Enter your height (cm): ", 1, 250);
        weight = getIntegerInput("Enter your weight (kg): ", 1, 200);
        int g = getIntegerInput("Enter 0 to select male, and 1 to select female: ", 0, 1);
        if(g == 0) gender = "Male";
        else gender = "Female";
        activity = "Moderate";
        UserProfile temp(age, height, weight, gender, activity);
        user = temp;
        user.saveRecords();
    }

    FoodDatabase database;
    DailyLog log;

    database.loadDatabase("food_database.txt");

    while (true) {
        cout << "\nMain Menu:\n"
             << "(1) Log Foods\n"
             << "(2) Manage Foods\n"
             << "(3) Manage Profile\n"
             << "(4) Exit\n";

        int option = getIntegerInput("Enter your choice: ", 1, 3);

        try {
            switch (option) {
                case 1:
                    logFoodsMenu(database, log, user);
                    break;
                case 2:
                    manageFoodsMenu(database);
                    break;
                case 3:
                    updateProfileMenu(user, log, database);
                    break;
                case 4:
                    database.saveDatabase("food_database.txt");
                    log.saveLog("daily_log.txt");
                    cout << "Exiting program.\n";
                    return 0;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}