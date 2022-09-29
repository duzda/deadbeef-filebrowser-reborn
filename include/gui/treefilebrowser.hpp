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
    void refreshThread();
    void stopThread();

    void getProgress(float* progress) const;

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


private:
    std::filesystem::path mTreeDirectory;
    Gtk::TreeView* mTreeView;
    Addressbox* mAddressbox;

    uint mIconSize = 32;

    bool mIsNeedleSet = true;
    bool refreshLock = false;

    TreeFilebrowser();
    ~TreeFilebrowser();

    float progressCount = 0;
    float progressIteration = 1;
    std::atomic<float> threadProgress = 0;
    

    /**
     * Function to refresh tree via second thread.
     * 
     * @param data pass this.
     */
    static void threadedRefreshTree(void* data);

    std::thread* mRefreshThread;
    std::atomic<bool> mRefreshThreadRun = true;

    /**
     * Recursively creates TreeModel at path.
     * 
     * @param path Creates TreeModel at path.
     * @param parent Parent of newly created entries(used in recursion).
     */
    void initialize(std::filesystem::path path, Gtk::TreeRow* parent = nullptr);

    void findChildren(std::filesystem::path path, Gtk::TreeNodeChildren child);

    /**
     * If tree would be empty, this adds line indicating that the directory is empty or hidden.
     */
    void checkEmptyRoot();

    /**
     * Probably wanna use new(), to not loose memory when sending to main thread.
     * 
     * @param self pass this.
     * @param row Make sure to initialize via new().
     * @param icon Icon.
     * @param entry File/directory path.
     * @param parent pass NULL, or parent row.
     */
    struct structRowData {
        TreeFilebrowser* self;
        Gtk::TreeRow* row;
        Glib::RefPtr<Gdk::Pixbuf> icon;
        std::filesystem::directory_entry entry;
        Gtk::TreeRow* parent;
    };

    /**
     * To be used via g_idle_add(), fills row of treeview on main thread.
     * 
     * @param data structRowData.
     * @return 0, to remove from idle queue.
     */
    static int fillRow(void* data);

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