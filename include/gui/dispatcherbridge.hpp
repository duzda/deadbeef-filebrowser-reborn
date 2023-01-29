#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Creates bridge just for the dispatcher between Addressbox and FBTreeModel.
 */
class GUI::DispatcherBridge {
public:
    DispatcherBridge();
    ~DispatcherBridge();

    void initialize(Addressbox* addressbox, Searchbar* searchbar, FBTreeView* view, FBTreeModel* model);

    bool inProgress();

    void onStart();

    /**
     * Notifies dispatcher.
     */
    void notify();
private:
    Addressbox* mAddressbox;
    Searchbar* mSearchbar;
    FBTreeView* mView;
    FBTreeModel* mModel;

    bool mInProgress = false;
    Glib::Dispatcher mDispatcher;

    /**
     * Changes button according to progress, updates progress bar, 
     * this is just a wild guess, and progress gets counted only by the root folders.
     */
    void updateProgressState();
};