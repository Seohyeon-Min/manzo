/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Logger.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    March 19, 2024
*/

#include <iostream>
#include <chrono>
#include "Logger.h"

CS230::Logger::Logger(Logger::Severity severity, bool use_console, std::chrono::system_clock::time_point start)
    : min_level(severity), out_stream("Trace.log"), start_time(start) {
    SetTraceLogLevel(LOG_NONE);
    if (use_console == true) {
        out_stream.set_rdbuf(std::cout.rdbuf());
        start_time = std::chrono::system_clock::now();
    }
}

CS230::Logger::~Logger() {
    out_stream.flush();
    out_stream.close();
}

const double CS230::Logger::seconds_since_start()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::duration<double>(now - start_time).count();
}

void CS230::Logger::log(CS230::Logger::Severity severity, std::string message) {
    const char* severity_names[] = { "Verbose", "Debug", "Event", "Error" };
    if (min_level <= severity)
    {
        out_stream << severity_names[static_cast<int>(severity)] << '\t' << message << '\n';
        out_stream.precision(4);
        out_stream << '[' << std::fixed << seconds_since_start() << "]\t";
    }

    return;
}