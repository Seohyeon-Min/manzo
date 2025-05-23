#pragma once
/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Logger.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <map>
#include <vec2.h>
enum LoggerLogLevel {  
    LOGNONE,
    LOGERROR,
    LOGWARNING,
    LOGINFO,
    LOGDEBUG
};

class Logger {
public:
    enum class Severity {
        Verbose,  //Minor messages
        Debug,    //Only used while actively debugging
        Event,    //General event, like key press or state change
        Error     //Errors, such as file load errors
    };
    Logger(Severity severity, bool use_console);
    Logger(std::chrono::system_clock::time_point start_time, Severity severity, bool use_console);
    ~Logger();
    void LogError(std::string text) {
        log(Severity::Error, text);
    }
    void LogEvent(std::string text) {
        log(Severity::Event, text);
    }
    void LogDebug(std::string text) {
        log(Severity::Debug, text);
    }
    void LogVerbose(std::string text) {
        log(Severity::Verbose, text);
    }
    void SetTraceLogLevel(LoggerLogLevel level) {
        currentLogLevel = level;
    }
    //void LoadSaveFile();
    //void WriteSaveFile(std::map<int, int> fishCollection, int money, bool module1_buy, bool module1_set, float m1x, bool module2_buy, bool module2_set, float m2x);

private:
    const std::string word[4]{ "Verbose","Debug","Event","Error" };
    Severity min_level;
    std::ofstream out_stream;
    std::chrono::system_clock::time_point start_time;
    double seconds_since_start();
    void log(Severity severity, std::string message);
    LoggerLogLevel currentLogLevel = LOGINFO;
};
