#pragma once

#include <gtkmm.h>

#include "treefilebrowser.hpp"
#include "searchbar.hpp"
#include "addressbox.hpp"
#include "filebrowserfilter.hpp"
#include "treepopup.hpp"

class Container : public Gtk::VBox {
public:
    Container();
    ~Container();
    void initialize();
private:
    Searchbar mSearchbar;
    Addressbox mAddressbox;
    Gtk::TreeView mTreeView;
    TreePopup mTreePopup;
    Gtk::ScrolledWindow mScrolledWindow;
    Glib::RefPtr<TreeFilebrowser> mTreeFilebrowser;
    Glib::RefPtr<FilebrowserFilter> mFilebrowserFilter;

    void buildTreeview();
};
