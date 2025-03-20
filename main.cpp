#include "UserProfile.h"
#include "FoodDatabase.h"
#include "DailyLog.h"
#include <iostream>

int main() {
    UserProfile user("Male", 25, 175, 70, "Moderate");
    FoodDatabase database;
    DailyLog log;

    database.addFood(Food("Apple", {"fruit", "apple"}, 95));
    database.addFood(Food("Chicken Breast", {"chicken", "protein"}, 165));

    string choice;
    cout << "Enter a food keyword to log: ";
    cin >> choice;
    Food* selectedFood = database.searchFood(choice);

    if (selectedFood) {
        int servings;
        cout << "Enter number of servings: ";
        cin >> servings;
        log.addFood(selectedFood->name, servings);
    } else {
        cout << "Food not found!\n";
    }

    log.displayLog();
    return 0;
}