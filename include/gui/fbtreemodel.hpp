#pragma once

#include <gtkmm.h>
#include <filesystem>
#include <thread>
#include <atomic>
#include <memory>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>

#include "gui.hpp"
#include "modelcolumns.hpp"
#include "fbtreeview.hpp"
#include "dispatcherbridge.hpp"

#include "plugin.hpp"

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
     * @return Double between 0 and 1, 1 when finished
     */
    double getProgress() const;

    /**
     * Sets new root directory, refreshTree should probably be called immediately after.
     * 
     * @param newDirectory New root directory
     */
    void setTreeRoot(const std::filesystem::path& newDirectory);

    /**
     * Creates new thread that refreshes tree, removes all entries, and fills them again. 
     * If you want to filter things out, see filebrowserfilter instead.
     * 
     * @param force Refresh can be forced, this is good idea if refresh is called due to an error, such as recovering from error while loading cached model
     */
    void refreshTree(bool force = false);

    /**
     * Restores Tree from cache
    */
    void initialLoad();

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
    void fillRow(const std::filesystem::directory_entry& entry, const Gtk::TreeNodeChildren* parent = nullptr);

    /**
     * Fills row of treeview, but is allowed to be exited prematurely. (This should be used when adding children of a node)
     * 
     * @param entry Path to the new child.
     * @param child Parent to the current entry.
     */
    void fillChildrenRow(const std::filesystem::directory_entry& entry, const Gtk::TreeNodeChildren* parent);

    /**
     * Fills information row to treeview.
     */
    void fillEmptyRow();

private:
    friend boost::serialization::access;

    struct Image {
        std::vector<guint8> data;
        // Colorspace (Add in the future)
        bool has_alpha; // System icons use alpha
        // Bits per sample (Add in the future)
        int size;
        int rowstride;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & data & size & rowstride & has_alpha;
        }
    };

    struct Row {
        int depth;
        std::string name;
        std::string uri;
        std::string tooltip;
        Image image;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & depth & name & uri & tooltip & image;
        }
    };

    // Implementation in modelserializer.cpp

    template<class Archive>
    void save(Archive &ar, const unsigned int version) const;

    template<class Archive>
    void saveRecursively(Archive &ar, std::vector<Row>* rows, Gtk::TreeIter iter, int depth, const unsigned int version) const;

    template<class Archive>
    void load(Archive &ar, const unsigned int version);

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};