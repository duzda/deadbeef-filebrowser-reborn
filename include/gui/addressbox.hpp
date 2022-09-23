#pragma once

#include <gtkmm.h>

#include "treefilebrowser.hpp"

class Addressbox : public Gtk::HBox
{
public:
    void setTreeFilebrowser(TreeFilebrowser* newTreeFilebrowser);
    void setAddress(std::string newAddres);
    std::string getAddress();

    /**
     * Makes path valid (removes trailing /)
     * 
     * @param path String we need to correct.
     * 
     * @return New corrected string.
     */
    std::string makeValidPath(std::string path);
    Addressbox();
    ~Addressbox();
private:
    Gtk::Button mGoButton;
    Gtk::Entry mAddressBar;
    TreeFilebrowser* mTreeFilebrowser;
    std::string mAddress;

    /**
     * Gets called when pressing Go! button
     */
    void on_go_button_click();
};