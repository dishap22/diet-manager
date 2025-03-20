#ifndef FOODDATABASE_H
#define FOODDATABASE_H

#include "Food.h"
#include <vector>
using namespace std;

class FoodDatabase {
public:
    vector<Food> foods;

    void addFood(Food food) {
        foods.push_back(food);
    }

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