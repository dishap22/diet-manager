#ifndef DAILYLOG_H
#define DAILYLOG_H

#include <map>
#include <string>
#include <iostream>
using namespace std;

class DailyLog {
public:
    map<string, int> log;

    void addFood(string food, int servings) {
        log[food] += servings;
    }

    void removeFood(string food) {
        log.erase(food);
    }

    void displayLog() {
        cout << "Today's log:\n";
        for (auto &entry : log) {
            cout << entry.first << " - " << entry.second << " servings\n";
        }
    }
};

#endif