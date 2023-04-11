#pragma once

#include "deadbeef/deadbeef.h"
#include <string>

enum LogLevel {
    Info,
    Warning,
    Error
};

void pluginLog(LogLevel level, const std::string& message);

extern DB_functions_t* deadbeef;