#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Filter for FBTreeModel, enables for fast searches set via Searchbar in the application.
 */
class GUI::FBTreeFilter : public Gtk::TreeModelFilter
{
public:
    FBTreeFilter(Glib::RefPtr<GUI::FBTreeModel> model);
    ~FBTreeFilter();

    static Glib::RefPtr<GUI::FBTreeFilter> create(Glib::RefPtr<GUI::FBTreeModel> model);
    void setNeedle(std::string needle);
private:
    std::string mNeedle;
    GUI::FBTreeModel* mModel;

    bool filter_func(Gtk::TreeIter iter);
};