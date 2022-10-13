#include "plugin.hpp"
#include "controller.hpp"
#include "settings.hpp"

#ifndef VMAJOR
    #define VMAJOR 0
#endif // VMAJOR

#ifndef VMINOR
    #define VMINOR 0
#endif // VMINOR

DB_functions_t* deadbeef = NULL;
static DB_misc_t plugin;

const char config_dialog[] =
"property \"Icon size: \" spinbtn[24,48,2] " FBR_ICON_SIZE " \"32\" ;\n"
"property \"Doubleclick replace playlist content \" checkbox " FBR_DOUBLECLICK_REPLACE " \"0\" ;\n"
"property \"Album icon algorithm: \" select[3] " FBR_ALBUM_ALGORITHM " \"0\" \"Cover first\" \"Metadata first\" \"Metadata only\";\n"
;

extern "C" 
DB_plugin_t* ddb_misc_filebrowser_reborn_load(DB_functions_t* api) {
    deadbeef = api;

    plugin.plugin.type = DB_PLUGIN_MISC;
    plugin.plugin.api_vmajor = 1;
    plugin.plugin.api_vminor = 15;
    plugin.plugin.version_major = VMAJOR;
    plugin.plugin.version_minor = VMINOR;
    plugin.plugin.id = "filebrowser-reborn";
    plugin.plugin.name = "File Browser Reborn";
    plugin.plugin.descr = "Simple file browser, based on http://sourceforge.net/projects/deadbeef-fb";
    plugin.plugin.copyright = "GPL-3.0";
    plugin.plugin.website = "https://www.github.com/duzda/deadbeef-filebrowser-reborn";
    plugin.plugin.connect = &Controller::pluginConnect;
    plugin.plugin.disconnect = &Controller::pluginDisconnect;
    plugin.plugin.flags = DDB_PLUGIN_FLAG_LOGGING;
    plugin.plugin.configdialog = config_dialog;
    plugin.plugin.message = &Controller::handleEvent;

    return DB_PLUGIN(&plugin.plugin);
}

void pluginLog(LogLevel level, std::string message) {
    std::string levelString = "UNKNOWN";

    int deadbeefLevel = 1;
    switch (level) {
        case LogLevel::Info:
            levelString = "INFO";
            break;
        case LogLevel::Warning:
            levelString = "WARNING";
            break;
        case LogLevel::Error:
            levelString = "ERROR";
            deadbeefLevel = 0;
            break;
    }

    std::string logMessage = "[Filebrowser Reborn] " + levelString + " - " + message + "\n";
    deadbeef->log_detailed(&plugin.plugin, deadbeefLevel, logMessage.c_str());
}