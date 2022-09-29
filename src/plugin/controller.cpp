#include "controller.hpp"
#include "plugin.hpp"
#include "settings.hpp"

ddb_gtkui_t* Controller::gtkui_plugin = nullptr;
Container* Controller::container = nullptr;

int Controller::pluginConnect() {
    Controller::gtkui_plugin = (ddb_gtkui_t*)deadbeef->plug_get_for_id(DDB_GTKUI_PLUGIN_ID);
    if (Controller::gtkui_plugin) {
        Controller::gtkui_plugin->w_reg_widget("File browser", DDB_WF_SINGLE_INSTANCE, &Controller::guiPluginConnect, "filebrowser-reborn", NULL);
        return 0;
    }

    return -1;
}

int Controller::pluginDisconnect() {
    Controller::guiPluginDisconnect();

    return 0;
}

ddb_gtkui_widget_t* Controller::guiPluginConnect() {
    Gtk::Main::init_gtkmm_internals();

    ddb_gtkui_widget_t* w = (ddb_gtkui_widget_t*)calloc(1, sizeof(ddb_gtkui_widget_t));
    w->widget = gtk_event_box_new();
    gtk_widget_set_can_focus(w->widget, false);

    Controller::container = new Container();
    gtk_container_add(GTK_CONTAINER(w->widget), (GtkWidget*)Controller::container->gobj());
    gtk_widget_show_all(w->widget);

    Controller::gtkui_plugin->w_override_signals(w->widget, w);

    return w;
}

int Controller::guiPluginDisconnect() {
    Controller::gtkui_plugin = nullptr;
    Controller::container = nullptr;

    return 0;
}