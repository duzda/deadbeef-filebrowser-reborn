#pragma once

#include <gtkmm.h>

#include "filebrowsermodel.hpp"
#include "filebrowserfilter.hpp"

/**
 * Extends Gtk::HBox, contains address bar and Go! button.
 * Styling happends in the constructor.
 */
class Addressbox : public Gtk::HBox
{
public:
    void initialize(Gtk::TreeView* treeview, Glib::RefPtr<FilebrowserFilter> filter, FilebrowserModel* treefb);
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
    FilebrowserModel* mFilebrowserModel;
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