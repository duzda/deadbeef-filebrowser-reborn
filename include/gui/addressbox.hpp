#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Extends Gtk::HBox, contains address bar and Go! button.
 * Styling happends in the constructor.
 */
class GUI::Addressbox : public Gtk::HBox
{
public:
    Addressbox();
    ~Addressbox();

    void initialize(GUI::DispatcherBridge* bridge, GUI::FBTreeView* view, FBTreeFilter* filter, FBTreeModel* model);
    void setAddress(std::string addres);
    std::string getAddress();

    /**
     * Changes progress of the action of Addressbox.
     * 
     * @param progress 0 - 1, where 1 is finished.
     */
    void setProgress(double progress);

    /**
     * Changes Addressbox sensitivity and icon.
     * 
     * @param running 
     */
    void setState(bool running);
private:
    Gtk::Button mGoButton;
    Gtk::Entry mAddressBar;
    GUI::FBTreeModel* mModel;
    GUI::FBTreeView* mView;
    GUI::FBTreeFilter* mFilter;
    GUI::DispatcherBridge* mBridge;

    /**
     * Gets called when pressing the search button.
     */
    void on_go_button_click();

    /**
     * Makes path valid (removes trailing /).
     * 
     * @param path String we need to correct.
     * 
     * @return New corrected string.
     */
    std::string makeValidPath(std::string path);
};