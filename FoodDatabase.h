#ifndef FOODDATABASE_H
#define FOODDATABASE_H

#include "Food.h"
#include "CompositeFood.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * Represents a database of food items.
 */
class FoodDatabase {
public:
    vector<Food*> foods;

    ~FoodDatabase() {
        for (auto food : foods) {
            delete food;
        }
    }

    /**
     * Adds a food item to the database.
     *
     * @param food The food item to add.
     */
    void addFood(Food* food) {
        foods.push_back(food);
    }

    /**
     * Adds a composite food item to the database.
     *
     * @param food The composite food item to add.
     */
    void addCompositeFood(CompositeFood* food) {
        foods.push_back(food);
    }

    /**
     * Searches for food items by keywords.
     *
     * @param keywords The list of keywords to search for.
     * @param matchAll If true, all keywords must match; if false, any keyword can match.
     * @return A vector of pointers to food items that match the criteria.
     */
    vector<Food*> searchFood(const vector<string>& keywords, bool matchAll) {
        vector<Food*> matchingFoods;

        // If keywords is empty, return all foods
        if (keywords.empty()) {
            return foods;
        }

        for (auto& food : foods) {
            int matchCount = 0;

            for (const auto& keyword : keywords) {
                if (find(food->keywords.begin(), food->keywords.end(), keyword) != food->keywords.end()) {
                    matchCount++;
                }
            }

            if ((matchAll && matchCount == keywords.size()) || (!matchAll && matchCount > 0)) {
                matchingFoods.push_back(food);
            }
        }

        return matchingFoods;
    }

    /**
     * Searches for a single food item by name.
     *
     * @param name The name of the food item to search for.
     * @return A pointer to the food item if found, or nullptr if not found.
     */
    Food* searchOneFood(const string& name) {
        for (auto& food : foods) {
            if (food->name == name) {
                return food;
            }
        }
        return nullptr; // Return nullptr if no matching food is found
    }

    /**
     * Displays all food items in the database.
     */
    void displayAllFoods() {
        cout << "Available foods:\n";
        for (int i = 0; i < foods.size(); ++i) {
            cout << i << ": " << foods[i]->name << " (" << foods[i]->calories << " calories) - " << (dynamic_cast<CompositeFood*>(foods[i]) ? "Composite" : "Basic") << endl;
        }
    }

    /**
     * Display some foods
     * 
     * @param foods The list of foods to display
     */
    void displayFoods(const vector<Food*>& foods) {
        cout << "Available foods:\n";
        for (int i = 0; i < foods.size(); ++i) {
            cout << i << ": " << foods[i]->name << " (" << foods[i]->calories << " calories) - " << (dynamic_cast<CompositeFood*>(foods[i]) ? "Composite" : "Basic") << endl;
        }
    }

    /**
     * Loads the database from a file.
     *
     * @param filename The name of the file to load the database from.
     */
    void loadDatabase(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No existing database found. Starting fresh.\n";
            return;
        }

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string type;
            getline(ss, type, '|');

            if (type == "B") {
                string name, keywordStr;
                int calories;
                getline(ss, name, '|');
                ss >> calories;
                ss.ignore();
                getline(ss, keywordStr);
                vector<string> keywords;
                stringstream ks(keywordStr);
                string keyword;
                while (getline(ks, keyword, ',')) {
                    keywords.push_back(keyword);
                }
                addFood(new Food(name, keywords, calories));
            } else if (type == "C") {
                string name, ingredientStr, keywordStr;
                getline(ss, name, '|');
                getline(ss, ingredientStr, '|');
                getline(ss, keywordStr);

                vector<CompositeFood::Ingredient> ingredients;
                stringstream ingStream(ingredientStr);
                string ingPair;
                while (getline(ingStream, ingPair, ';')) {
                    stringstream ingSS(ingPair);
                    string foodName;
                    int servings;
                    getline(ingSS, foodName, ',');
                    ingSS >> servings;

                    Food* foundFood = searchOneFood(foodName);
                    if (foundFood) {
                        ingredients.push_back({foundFood, servings});
                    }
                }

                vector<string> keywords;
                stringstream ks(keywordStr);
                string keyword;
                while (getline(ks, keyword, ',')) {
                    keywords.push_back(keyword);
                }
                addCompositeFood(new CompositeFood(name, ingredients, keywords));
            }
        }
        file.close();
        cout << "Database loaded successfully.\n";
    }

    /**
     * Saves the database to a file.
     *
     * @param filename The name of the file to save the database to.
     */
    void saveDatabase(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error saving database!\n";
            return;
        }

        if (!file.is_open()) {
            cerr << "Error: Could not create file " << filename << endl;
            return;
        }

        for (auto& food : foods) {
            if (auto* composite = dynamic_cast<CompositeFood*>(food)) {
                file << "C|" << composite->name << "|";
                for (size_t i = 0; i < composite->ingredients.size(); ++i) {
                    file << composite->ingredients[i].food->name << "," << composite->ingredients[i].servings;
                    if (i < composite->ingredients.size() - 1) file << ";";
                }
                file << "|";
                for (size_t i = 0; i < composite->keywords.size(); ++i) {
                    file << composite->keywords[i];
                    if (i < composite->keywords.size() - 1) file << ",";
                }
                file << "\n";
            } else {
                file << "B|" << food->name << "|" << food->calories << "|";
                for (size_t i = 0; i < food->keywords.size(); ++i) {
                    file << food->keywords[i];
                    if (i < food->keywords.size() - 1) file << ",";
                }
                file << "\n";
            }
        }
        file.close();
        cout << "Database saved successfully.\n";
    }
};

#endif