#pragma once

#include <gtkmm.h>
#include <filesystem>
#include <thread>
#include <atomic>
#include <memory>

#include "modelcolumns.hpp"

class Addressbox;

class TreeFilebrowser : public Gtk::TreeStore {
public:
    ModelColumns mModelColumns;
    static Glib::RefPtr<TreeFilebrowser> create();
    void setNeedleState(bool newState);
    void initialize(Gtk::TreeView* treeview, Addressbox* addressbox);
    void setIconSize(uint newValue);

    /**
     * Returns approximate progress of reading directory tree.
     * 
     * @return Float between 0 and 1, 1 when finished
     */
    float getProgress();

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
    TreeFilebrowser();
    ~TreeFilebrowser();

    std::filesystem::path mTreeDirectory;
    Gtk::TreeView* mTreeView;
    Addressbox* mAddressbox;

    uint mIconSize = 32;

    bool mIsNeedleSet = false;

    bool mRefreshLock = false;

    std::thread* mRefreshThread;
    std::atomic<float> mThreadProgress = 0;
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
     * @param child Parent to of the current entry.
     */
    void fillChildrenRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent);

    /**
     * To be used via g_idle_add(), fills information row to treeview on main thread.
     * 
     * @param data pass this.
     * @return 0, to remove from idle queue.
     */
    static int fillEmptyRow(void* data);

    /**
     * To be used via g_idle_add(), clears all rows.
     * 
     * @param data pass this.
     * @return 0, to remove from idle queue.
     */
    static int clearRows(void* data);
};