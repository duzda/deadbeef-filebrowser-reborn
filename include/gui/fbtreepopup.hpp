#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Extends Gtk::Menu, container for on click events for TreeView
 */
class GUI::FBTreePopup : public Gtk::Menu {
public:
    FBTreePopup();
    ~FBTreePopup();

    /**
     * Binds events to be callable etc.
     */
    void initialize(GUI::FBTreeView* view, GUI::FBTreeModel* model, GUI::FBTreeFilter* filter, GUI::Addressbox* addressbox);
private:
    /**
     * Function arguments for addToPlaylist when forking thread
     */
    struct structAddToPlaylist {
        std::vector<std::string> uris;
        std::string address;
        bool replace;
    };

    structAddToPlaylist mDataHolder;

    Glib::RefPtr<Gio::Menu> mMenu;
    Glib::RefPtr<Gio::SimpleActionGroup> mActionGroup;
    GUI::FBTreeView* mView;
    GUI::FBTreeModel* mModel;
    GUI::FBTreeFilter* mFilter;
    GUI::Addressbox* mAddressbox;

    bool on_click(GdkEventButton* event);

    // Various events for the popup
    void popup_add();
    void popup_replace();
    void popup_open_folder();
    void popup_enter_directory();
    void popup_go_up();
    void popup_refresh();

    /**
     * Adds to playlist files or folders.
     * 
     * @param uris URIs of selected rows.
     * @param address Every URI has to be constructed first, pass mAddressbox->getAddress().
     * @param replace Whether current playlist should be replaced.
     */
    static void addToPlaylist(std::vector<std::string> uris, std::string address, bool replace);

    /**
     * Calls addToPlaylist, make sure to pass structAddToPlaylist as argument.
     */
    static void threadedAddToPlaylist(void* ctx);

    /**
     * Calls addToPlaylist, make sure to pass structAddToPlaylist as argument.
     */
    static void threadedReplacePlaylist(void* ctx);
};