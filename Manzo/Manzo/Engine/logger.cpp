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

CS230::Logger::Logger(Logger::Severity severity, bool use_console) : min_level(severity), out_stream("Trace.log") {
    SetTraceLogLevel(LOGNONE);
    if (use_console == true) {
        out_stream.set_rdbuf(std::cout.rdbuf());
    }
}

CS230::Logger::Logger(std::chrono::system_clock::time_point start_t, Logger::Severity severity, [[maybe_unused]]bool use_console) : Logger(severity, "Trace.log") {
    start_time = start_t;
}

CS230::Logger::~Logger() {
    out_stream.flush();
    out_stream.close();
}

void CS230::Logger::log(CS230::Logger::Severity severity, std::string message) {

    if (severity >= min_level) {
        out_stream.precision(4);
        out_stream << '[' << std::fixed << seconds_since_start() << "]\t";
        out_stream << word[(int)severity] + "\t" + message + "\n";

    }
}

double CS230::Logger::seconds_since_start() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double since_start = std::chrono::duration<double>(now - start_time).count();
    return since_start;
}
