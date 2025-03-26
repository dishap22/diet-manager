#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
using namespace std;

/**
 * Represents a food item with a name, keywords, and calorie count.
 */
class Food {
public:
    string name;
    vector<string> keywords;
    int calories;

    /**
     * Constructs a Food object with the given name, keywords, and calorie count.
     *
     * @param n The name of the food.
     * @param k A list of descriptive keywords.
     * @param c The number of calories.
     */
    Food(string n, vector<string> k, int c) : name(n), keywords(k), calories(c) {}
};

#endif