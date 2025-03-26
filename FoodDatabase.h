#ifndef FOODDATABASE_H
#define FOODDATABASE_H

#include "Food.h"
#include <vector>
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
     * Searches for a food item by keyword.
     *
     * @param keyword The keyword to search for.
     * @return A pointer to the first food item that contains the keyword, or nullptr if no such food item exists.
     */
    Food* searchFood(string keyword) {
        for (auto &food : foods) {
            for (auto &k : food.keywords) {
                if (k == keyword) return &food;
            }
        }
        return nullptr;
    }
};

#endif