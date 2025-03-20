#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
using namespace std;

class Food {
public:
    string name;
    vector<string> keywords;
    int calories;

    Food(string n, vector<string> k, int c) : name(n), keywords(k), calories(c) {}
};

#endif