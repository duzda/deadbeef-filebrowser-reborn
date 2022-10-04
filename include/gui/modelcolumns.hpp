#pragma once

#include <gtkmm.h>

#include "gui.hpp"

class GUI::ModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
    ModelColumns();

    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> ColumnIcon;
    Gtk::TreeModelColumn<std::string> ColumnName;
    Gtk::TreeModelColumn<std::string> ColumnURI;
    Gtk::TreeModelColumn<std::string> ColumnTooltip;
    Gtk::TreeModelColumn<bool> ColumnVisibility;
};