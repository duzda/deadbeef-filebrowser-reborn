#pragma once

#include <gtkmm.h>

class ModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
    ModelColumns();
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> mColumnIcon;
    Gtk::TreeModelColumn<std::string> mColumnName;
    Gtk::TreeModelColumn<std::string> mColumnURI;
    Gtk::TreeModelColumn<std::string> mColumnTooltip;
    Gtk::TreeModelColumn<bool> mColumnVisibility;
};