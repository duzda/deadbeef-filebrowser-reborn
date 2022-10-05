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
    static int handleEvent(uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2);
private:
    static ddb_gtkui_t* gtkui_plugin;
    static GUI::Container* container;

    Controller();
    ~Controller();
};