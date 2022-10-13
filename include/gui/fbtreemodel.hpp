#pragma once

#include <gtkmm.h>
#include <filesystem>
#include <thread>
#include <atomic>
#include <memory>

#include "gui.hpp"
#include "modelcolumns.hpp"

/**
 * Extends Gtk::TreeStore, container for all the data, see ModelColumns for used columns.
 */
class GUI::FBTreeModel : public Gtk::TreeStore {
public:
    GUI::ModelColumns ModelColumns;
    static Glib::RefPtr<GUI::FBTreeModel> create();
    void setNeedleState(bool state);
    void initialize(GUI::DispatcherBridge* bridge, GUI::FBTreeView* view);

    /**
     * Returns approximate progress of reading directory tree.
     * 
     * @return Float between 0 and 1, 1 when finished
     */
    double getProgress();

    /**
     * Sets new root directory and refreshes tree.
     * 
     * @param newDirectory New root directory
     */
    void setTreeRoot(std::filesystem::path newDirectory);

    /**
     * Creates new thread that refreshes tree, removes all entries, and fills them again. 
     * If you want to filter things out, see filebrowserfilter instead.
     */
    void refreshTree();

    void refreshThread();

    /**
     * Stops the thread that updates the tree directory.
     */
    void stopThread();

private:
    FBTreeModel();
    ~FBTreeModel();

    std::filesystem::path mTreeDirectory;
    GUI::FBTreeView* mView;
    GUI::DispatcherBridge* mBridge;

    bool mIsNeedleSet = false;

    bool mRefreshLock = false;
    std::thread* mRefreshThread;
    std::atomic<double> mThreadProgress = 0;
    std::atomic<bool> mRefreshThreadRunning = true;

    /**
     * If tree would be empty, this adds line indicating that the directory is empty or hidden.
     */
    void checkEmptyRoot();

    /**
     * Fills row of treeview.
     * 
     * @param entry Path to the new child.
     * @param child Parent to of the current entry, can be null.
     */
    void fillRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent = nullptr);

    /**
     * Fills row of treeview, but is allowed to be exited prematurely. (This should be used when adding children of a node)
     * 
     * @param entry Path to the new child.
     * @param child Parent to the current entry.
     */
    void fillChildrenRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent);

    /**
     * Fills information row to treeview.
     */
    void fillEmptyRow();
};