#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"

class TreeView : public Gtk::TreeView {
public:
    TreeView();
    ~TreeView();

    void initialize(FilebrowserModel* fbmodel);
private:
};