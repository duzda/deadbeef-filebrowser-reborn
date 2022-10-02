#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"
#include "searchbar.hpp"
#include "addressbox.hpp"
#include "treeview.hpp"
#include "filebrowserfilter.hpp"
#include "treepopup.hpp"

/**
 * Extends Gtk::VBox, main container for the application, also bootstraps all other controls.
 */
class Container : public Gtk::VBox {
public:
    Container();
    ~Container();
private:
    Searchbar mSearchbar;
    Addressbox mAddressbox;
    TreeView mTreeView;
    TreePopup mTreePopup;
    Gtk::ScrolledWindow mScrolledWindow;
    Glib::RefPtr<FilebrowserModel> mFilebrowserModel;
    Glib::RefPtr<FilebrowserFilter> mFilebrowserFilter;

    void initialize();
};
