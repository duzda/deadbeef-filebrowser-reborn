#pragma once

#include "deadbeef/deadbeef.h"
#include <string>


void pluginLog(int level, std::string message);

extern DB_functions_t* deadbeef;