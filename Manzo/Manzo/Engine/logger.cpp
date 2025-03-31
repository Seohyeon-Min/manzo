/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Logger.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    March 8, 2023
Updated:    March 10, 2023
*/

#include <iostream>
#include "Logger.h"

Logger::Logger(Logger::Severity severity, bool use_console) : min_level(severity), out_stream("Trace.log") {
    SetTraceLogLevel(LOGNONE);
    if (use_console == true) {
        out_stream.set_rdbuf(std::cout.rdbuf());
    }
}

Logger::Logger(std::chrono::system_clock::time_point start_t, Logger::Severity severity, [[maybe_unused]]bool use_console) : Logger(severity, "Trace.log") {
    start_time = start_t;
}

Logger::~Logger() {
    out_stream.flush();
    out_stream.close();
}

void Logger::log(Logger::Severity severity, std::string message) {

    if (severity >= min_level) {
        out_stream.precision(4);
        out_stream << '[' << std::fixed << seconds_since_start() << "]\t";
        out_stream << word[(int)severity] + "\t" + message + "\n";

    }
}

void Logger::LoadSaveFile()
{
    std::ifstream file("assets/scenes/save_data.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    int fishType, count;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        if (ss >> fishType >> count) {
            fiShCollection[fishType - 1] = count;
        }
        else if (line.find("Money:") != std::string::npos) {
            std::istringstream ss_money(line.substr(7));
            ss_money >> money;
        }
        else if (line.find("Module1:") != std::string::npos) {
            int module1Value;
            float module1x;
            std::istringstream ss_module(line.substr(9));
            ss_module >> module1Value >> module1x;

            if (module1Value == 1)
            {
                module1 = true;
                module1Pos = module1x;
            }
        }
        else if (line.find("Module2:") != std::string::npos) {
            int module2Value;
            float module2x;
            std::istringstream ss_module(line.substr(9));
            ss_module >> module2Value >> module2x;

            if (module2Value == 1)
            {
                module2 = true;
                module2Pos = module2x;
            }
        }
    }

    file.close();
}

void Logger::WriteSaveFile(std::map<int, int> fishCaptureCount, int money, bool module1, float m1x, bool module2, float m2x)
{
    std::ofstream saveFile("assets/scenes/save_data.txt", std::ios::trunc);

    if (saveFile.is_open()) {

        for (const auto& entry : fishCaptureCount) {
            saveFile << entry.first + 1 << " " << entry.second << "\n";
        }

        saveFile << "Money: " << std::to_string(money) << "\n";
        saveFile << "Module1: " << std::to_string(module1) << " " << std::to_string((module1 == true) ? m1x : -130) << "\n";
        saveFile << "Module2: " << std::to_string(module2) << " " << std::to_string((module2 == true) ? m2x : 0) << "\n";
        saveFile.close();
    }
}

double Logger::seconds_since_start() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double since_start = std::chrono::duration<double>(now - start_time).count();
    return since_start;
}
