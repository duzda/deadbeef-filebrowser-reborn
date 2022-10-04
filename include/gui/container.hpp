#pragma once

#include <gtkmm.h>

#include "gui.hpp"
#include "dispatcherbridge.hpp"
#include "searchbar.hpp"
#include "addressbox.hpp"
#include "fbtreeview.hpp"
#include "fbtreepopup.hpp"

/**
 * Extends Gtk::VBox, main container for the application, also bootstraps all other controls.
 */
class GUI::Container : public Gtk::VBox {
public:
    Container();
    ~Container();
private:
    GUI::DispatcherBridge mBridge;
    GUI::Searchbar mSearchbar;
    GUI::Addressbox mAddressbox;
    GUI::FBTreeView mView;
    GUI::FBTreePopup mTreePopup;
    Gtk::ScrolledWindow mScrolledWindow;
    Glib::RefPtr<GUI::FBTreeModel> mModel;
    Glib::RefPtr<GUI::FBTreeFilter> mFilter;

    void initialize();
};
