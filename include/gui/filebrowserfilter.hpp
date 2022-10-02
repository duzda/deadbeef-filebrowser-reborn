#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"

class FilebrowserFilter : public Gtk::TreeModelFilter
{
public:
    FilebrowserFilter(Glib::RefPtr<Gtk::TreeModel> model);
    ~FilebrowserFilter();
    static Glib::RefPtr<FilebrowserFilter> create(Glib::RefPtr<FilebrowserModel> treefb);
    void setNeedle(std::string newNeedle);
    void setModel(FilebrowserModel* treefb);
private:
    std::string mNeedle;
    FilebrowserModel* mFilebrowserModel;

    bool filter_func(Gtk::TreeIter iter);
};