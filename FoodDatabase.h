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
     * Searches for a food item by keyword.
     *
     * @param keyword The keyword to search for.
     * @return A pointer to the first food item that contains the keyword, or nullptr if no such food item exists.
     */
    Food* searchFood(string keyword) {
        for (auto &food : foods) {
            if (food->name == keyword) return food;

            for (auto &k : food->keywords) {
                if (k == keyword) return food;
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
            cout << i << ": " << foods[i]->name << " (" << foods[i]->calories << " calories) - " << (dynamic_cast<CompositeFood*>(foods[i]) ? "Composite" : "Basic") << endl;
        }
    }

    /**
     * Saves the database to a file.
     *
     * @param filename The name of the file to save the database to.
     */
    void loadDatabase(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No existing database found. Starting fresh.\n";
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

                    Food* foundFood = searchFood(foodName);
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