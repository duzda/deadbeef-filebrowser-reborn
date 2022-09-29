#pragma once

#include <gtkmm.h>

#include "treefilebrowser.hpp"
#include "filebrowserfilter.hpp"

class Addressbox : public Gtk::HBox
{
public:
    void setTreeFilebrowser(TreeFilebrowser* newTreeFilebrowser);
    void setAddress(std::string newAddres);
    void updateProgress(float progress);
    void initialize(Gtk::TreeView* treeview, Glib::RefPtr<FilebrowserFilter>);
    std::string getAddress();

    /**
     * Makes path valid (removes trailing /)
     * 
     * @param path String we need to correct.
     * 
     * @return New corrected string.
     */
    std::string makeValidPath(std::string path);

    // Called from the worker thread.
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

    bool inProgress = false;

    Glib::Dispatcher mDispatcher;
    void onNotify();
    void updateProgress();

    /**
     * Gets called when pressing Go! button
     */
    void on_go_button_click();

    struct ProgressBarData {
        Gtk::Entry* bar;
        float progress;
    };
    static void setProgressBar(ProgressBarData* data);
};