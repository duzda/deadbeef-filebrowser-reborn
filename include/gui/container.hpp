#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"
#include "searchbar.hpp"
#include "addressbox.hpp"
#include "filebrowserfilter.hpp"
#include "treepopup.hpp"

class Container : public Gtk::VBox {
public:
    Container();
    ~Container();
private:
    Searchbar mSearchbar;
    Addressbox mAddressbox;
    Gtk::TreeView mTreeView;
    TreePopup mTreePopup;
    Gtk::ScrolledWindow mScrolledWindow;
    Glib::RefPtr<FilebrowserModel> mFilebrowserModel;
    Glib::RefPtr<FilebrowserFilter> mFilebrowserFilter;

    void initialize();
    void buildTreeview();
};
