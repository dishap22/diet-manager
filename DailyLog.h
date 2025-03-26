#ifndef DAILYLOG_H
#define DAILYLOG_H

#include <map>
#include <string>
#include <iostream>
using namespace std;

/**
 * Represents a daily log of food items consumed.
 */
class DailyLog {
public:
    map<string, int> log;

    /**
     * Adds the given food to the log with the specified number of servings.
     *
     * @param food The name of the food.
     * @param servings The number of servings consumed.
     */
    void addFood(string food, int servings) {
        log[food] += servings;
    }

    /**
     * Removes the given food from the log.
     *
     * @param food The name of the food.
     */
    void removeFood(string food) {
        log.erase(food);
    }

    /**
     * Displays the log of food items consumed.
     */
    void displayLog() {
        cout << "Today's log:\n";
        for (auto &entry : log) {
            cout << entry.first << " - " << entry.second << " servings\n";
        }
    }
};

#endif