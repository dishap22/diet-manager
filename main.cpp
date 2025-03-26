// Updated main.cpp
#include "UserProfile.h"
#include "FoodDatabase.h"
#include "DailyLog.h"
#include <iostream>

int main() {
    UserProfile user("DummyUser", "Male", 25, 175, 70, "Moderate");
    FoodDatabase database;
    DailyLog log;

    database.addFood(new Food("Apple", {"fruit", "apple"}, 95));
    database.addFood(new Food("Chicken Breast", {"chicken", "protein"}, 165));
    database.addFood(new Food("Bread", {"bread", "carbs"}, 80));
    database.addFood(new Food("Peanut Butter", {"peanut", "butter", "protein"}, 190));
    database.addFood(new Food("Jelly", {"jelly", "sweet"}, 50));

    while (true) {
        cout << "\nChoose an option:\n"
             << "(1) Log food\n"
             << "(2) Create composite food\n"
             << "(3) View all foods\n"
             << "(4) Add new basic food\n"
             << "(5) Exit\n"
             << "Enter your choice: ";
        int option;
        cin >> option;

        if (option == 1) {
            string choice;
            cout << "Enter a food name to log: ";
            cin.ignore();
            getline(cin, choice);
            Food* selectedFood = database.searchFood(choice);

            if (selectedFood) {
                int servings;
                cout << "Enter number of servings: ";
                cin >> servings;
                log.addFood(selectedFood->name, servings);
            } else {
                cout << "Food not found!\n";
            }
        }
        else if (option == 2) {
            string compositeName;
            vector<CompositeFood::Ingredient> ingredients;
            vector<string> keywords;

            cout << "Enter name for the new composite food: ";
            cin.ignore();
            getline(cin, compositeName);

            cout << "Enter optional keywords (separated by commas, leave empty to generate from ingredients): ";
            string keywordInput;
            getline(cin, keywordInput);
            if (!keywordInput.empty()) {
                size_t pos = 0;
                while ((pos = keywordInput.find(',')) != string::npos) {
                    string kw = keywordInput.substr(0, pos);
                    keywords.push_back(kw);
                    keywordInput.erase(0, pos + 1);
                }
                keywords.push_back(keywordInput);
            }

            database.displayAllFoods();

            cout << "Add ingredients (enter index and servings, end with -1):\n";
            while (true) {
                int index;
                cout << "Food index (-1 to finish): ";
                cin >> index;
                if (index == -1) break;

                if (index < 0 || index >= database.foods.size()) {
                    cout << "Invalid index. Try again.\n";
                    continue;
                }

                int servings;
                cout << "Number of servings: ";
                cin >> servings;

                ingredients.push_back({database.foods[index], servings});
            }

            if (!ingredients.empty()) {
                CompositeFood* newComposite = new CompositeFood(compositeName, ingredients, keywords);
                database.addCompositeFood(newComposite);
                cout << "Composite food created: " << compositeName
                     << " with " << newComposite->calories << " calories.\n";
            } else {
                cout << "No ingredients selected. Composite food not created.\n";
            }
        }
        else if (option == 3) {
            database.displayAllFoods();
        }
        else if (option == 4) {
            string name;
            vector<string> keywords;
            int calories;

            cout << "Enter food name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter keywords (separated by commas): ";
            string keywordInput;
            getline(cin, keywordInput);
            size_t pos = 0;
            while ((pos = keywordInput.find(',')) != string::npos) {
                string kw = keywordInput.substr(0, pos);
                keywords.push_back(kw);
                keywordInput.erase(0, pos + 1);
            }
            keywords.push_back(keywordInput);

            cout << "Enter calories: ";
            cin >> calories;

            database.addFood(new Food(name, keywords, calories));
            cout << "New basic food added: " << name << endl;
        }
        else if (option == 5) {
            break;
        }
        else {
            cout << "Invalid option, try again.\n";
        }
    }

    log.displayLog();
    return 0;
}