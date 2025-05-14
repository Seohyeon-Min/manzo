#pragma once

#include <string>
#include <vector>
#include <map>
//#include <iostream>
//#include <sstream>
//#include <fstream>

struct ModuleData {
    bool buy = false;
    bool set = false;
    float pos = 0.0f;
};

struct SaveData {
    int day;
    int money;
    double user_calibration;

    std::vector<std::string> eventsDone;

    std::map<int, int> fishCollection;

    ModuleData module1;
    ModuleData module2;
    //float module1Pos = 285.0f;
    //float module2Pos = 415.0f;
};

