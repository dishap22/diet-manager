#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
using namespace std;

class UserProfile {
public:
    string gender;
    int age, height, weight;
    string activityLevel;

    UserProfile(string g, int a, int h, int w, string activity) : gender(g), age(a), height(h), weight(w), activityLevel(activity) {}
};

#endif