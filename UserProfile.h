#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
using namespace std;

/**
 * Represents a user with a username, age, height, weight, gender and activity level.
 */
class UserProfile {
public:
    string username;
    string gender;
    int age, height, weight;
    string activityLevel;

    /**
     * Constructs a UserProfile object with the given gender, age, height, weight, and activity level.
     *
     * @param username The username of the user
     * @param g The gender of the user
     * @param a The age of the user
     * @param h The height of the user
     * @param w The weight of the user
     * @param activity The activity level of the user
     */
    UserProfile(string username, string g, int a, int h, int w, string activity) : gender(g), age(a), height(h), weight(w), activityLevel(activity) {}
};

#endif