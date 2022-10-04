#pragma once

#include <gtkmm.h>

#include "gui.hpp"

class GUI::DispatcherBridge {
public:
    DispatcherBridge();
    ~DispatcherBridge();

    void initialize(Addressbox* addressbox, FBTreeView* view, FBTreeModel* model);

    bool inProgress();

    /**
     * Notifies dispatcher.
     */
    void notify();
private:
    Addressbox* mAddressbox;
    FBTreeView* mView;
    FBTreeModel* mModel;

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
};