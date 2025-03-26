#ifndef FOODDATABASE_H
#define FOODDATABASE_H

#include "Food.h"
#include "CompositeFood.h"
#include <vector>
#include <iostream>
using namespace std;

/**
 * Represents a database of food items.
 */
class FoodDatabase {
public:
    vector<Food> foods;

    /**
     * Adds a food item to the database.
     *
     * @param food The food item to add.
     */
    void addFood(Food food) {
        foods.push_back(food);
    }

    /**
     * Adds a composite food item to the database.
     *
     * @param food The composite food item to add.
     */
    void addCompositeFood(CompositeFood food) {
        foods.push_back(food);
    }

    /**
     * Searches for a food item by keyword.
     *
     * @param keyword The keyword to search for.
     * @return A pointer to the first food item that contains the keyword, or nullptr if no such food item exists.
     */
    Food* searchFood(string keyword) {
        for (auto &food : foods) {
            if (food.name == keyword) return &food;

            for (auto &k : food.keywords) {
                if (k == keyword) return &food;
            }
        }
        return nullptr;
    }

    /**
     * Displays all food items in the database.
     */
    void displayAllFoods() {
        cout << "Available foods:\n";
        for (int i = 0; i < foods.size(); ++i) {
            cout << i << ": " << foods[i].name << " (" << foods[i].calories << " calories)\n";
        }
    }
};

#endif