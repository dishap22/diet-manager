#ifndef COMPOSITEFOOD_H
#define COMPOSITEFOOD_H

#include "Food.h"

/**
 * Represents a composite food made of multiple food items.
 */
class CompositeFood : public Food {
public:
    vector<Food> ingredients;

    /**
     * Constructs a composite food with the given name and ingredients.
     *
     * @param n The name of the food.
     * @param ing The ingredients of the food.
     */
    CompositeFood(string n, vector<Food> ing) : Food(n, {}, 0), ingredients(ing) {
        for (auto &food : ingredients) {
            calories += food.calories;
        }
    }
};

#endif