#pragma once

#include <gtkmm.h>

#include "treefilebrowser.hpp"
#include "filebrowserfilter.hpp"

class Addressbox : public Gtk::HBox
{
public:
    void initialize(Gtk::TreeView* treeview, Glib::RefPtr<FilebrowserFilter> filter, TreeFilebrowser* treefb);
    void setAddress(std::string newAddres);
    std::string getAddress();

    /**
     * Notifies dispatcher.
     */
    void notify();

    Addressbox();
    ~Addressbox();
private:
    Gtk::Button mGoButton;
    Gtk::Entry mAddressBar;
    TreeFilebrowser* mTreeFilebrowser;
    Gtk::TreeView* mTreeView;
    Glib::RefPtr<FilebrowserFilter> mFilebrowserFilter;
    std::string mAddress;

    bool mInProgress = false;
    Glib::Dispatcher mDispatcher;

    /**
     * Dispatcher event.
     */
    void onNotify();

    /**
     * Changes button according to progress, updates progress bar, 
     * this is just a wild guess, and progress gets counted only by the root folders.
     */
    void updateProgressState();

    /**
     * Gets called when pressing the search button
     */
    void on_go_button_click();

    /**
     * Makes path valid (removes trailing /)
     * 
     * @param path String we need to correct.
     * 
     * @return New corrected string.
     */
    std::string makeValidPath(std::string path);
};