#include "UserProfile.h"
#include "FoodDatabase.h"
#include "DailyLog.h"
#include <iostream>

int main() {
    UserProfile user("DummyUser", "Male", 25, 175, 70, "Moderate");
    FoodDatabase database;
    DailyLog log;

    database.addFood(Food("Apple", {"fruit", "apple"}, 95));
    database.addFood(Food("Chicken Breast", {"chicken", "protein"}, 165));
    database.addFood(Food("Bread", {"bread", "carbs"}, 80));
    database.addFood(Food("Peanut Butter", {"peanut", "butter", "protein"}, 190));
    database.addFood(Food("Jelly", {"jelly", "sweet"}, 50));

    CompositeFood pbSandwich("Peanut Butter Sandwich", {database.foods[2], database.foods[3]});
    database.addCompositeFood(pbSandwich);

    CompositeFood pbjSandwich("Peanut Butter and Jelly Sandwich", {pbSandwich, database.foods[4]});
    database.addCompositeFood(pbjSandwich);

    while (true) {
        cout << "Choose an option: (1) Log food, (2) Create composite food, (3) View all foods, (4) Exit: ";
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
            vector<Food> ingredients;

            cout << "Enter name for the new composite food: ";
            cin.ignore();
            getline(cin, compositeName);

            database.displayAllFoods();

            cout << "Enter indices of foods to combine (separated by spaces, end with -1): ";
            int index;
            while (cin >> index && index != -1) {
                if (index >= 0 && index < database.foods.size()) {
                    ingredients.push_back(database.foods[index]);
                } else {
                    cout << "Invalid index. Try again.\n";
                }
            }

            if (!ingredients.empty()) {
                CompositeFood newComposite(compositeName, ingredients);
                database.addCompositeFood(newComposite);
                cout << "Composite food created: " << compositeName << " with " << newComposite.calories << " calories.\n";
            } else {
                cout << "No valid ingredients selected.\n";
            }
        }
        else if (option == 3) {
            database.displayAllFoods();
        }
        else if (option == 4) {
            break;
        }
        else {
            cout << "Invalid option, try again.\n";
        }
    }

    log.displayLog();
    return 0;
}