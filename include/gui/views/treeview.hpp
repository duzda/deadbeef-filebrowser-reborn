#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"

/**
 * Extends Gtk::TreeView, most of the styling happends during initialization.
 */
class TreeView : public Gtk::TreeView {
public:
    TreeView();
    ~TreeView();

    void initialize(FilebrowserModel* fbmodel);
private:
};