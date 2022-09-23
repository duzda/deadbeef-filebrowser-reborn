#pragma once

#include <cstdint>
#include <gtkmm.h>

#include "deadbeef/deadbeef.h"
#include "deadbeef/gtkui_api.h"

#include "container.hpp"

class Controller
{
public:
    static int pluginConnect();
    static int pluginDisconnect();
    static ddb_gtkui_widget_t* guiPluginConnect();
    static int guiPluginDisconnect();
private:
    static ddb_gtkui_t* gtkui_plugin;
    static Container* container;

    Controller();
    ~Controller();
};