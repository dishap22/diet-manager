#ifndef COMPOSITEFOOD_H
#define COMPOSITEFOOD_H

#include "Food.h"
#include <algorithm>

/**
 * Represents a composite food made of multiple food items.
 */
class CompositeFood : public Food {
public:
    struct Ingredient {
        Food* food;
        int servings;
    };
    vector<Ingredient> ingredients;

    /**
     * Constructs a composite food with the given name and ingredients.
     *
     * @param n The name of the food.
     * @param ing The ingredients of the food.
     */
    CompositeFood(string n, vector<Ingredient> ing, vector<string> k = {}) : Food(n, k, 0), ingredients(ing) {
        for (auto &ingredient : ingredients) {
            calories += ingredient.food -> calories * ingredient.servings;
        }

        if (k.empty()) {
            for (auto &ingredient : ingredients) {
                for (auto &keyword : ingredient.food -> keywords) {
                    keywords.push_back(keyword);
                }
            }

            sort(keywords.begin(), keywords.end());
            keywords.erase(unique(keywords.begin(), keywords.end()), keywords.end());
        }
    }
};

#endif