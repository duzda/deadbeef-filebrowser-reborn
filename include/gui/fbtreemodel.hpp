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
    double getProgress();

    /**
     * Sets new root directory, refreshTree should probably be called immediately after.
     * 
     * @param newDirectory New root directory
     */
    void setTreeRoot(std::filesystem::path newDirectory);

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

private:
    friend boost::serialization::access;

    struct Image {
        std::vector<guint8> data;
        // Colorspace (Add in the future)
        // Has Alpha (Might add? but it's always 0)
        // Bits per sample (Add in the future)
        int size;
        int rowstride;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & data & size & rowstride;
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

    template<class Archive>
    void save(Archive &ar, const unsigned int version) const {
        std::vector<Row> rows;
        for (auto iter = this->children().begin(); iter != this->children().end(); iter++) {
            this->saveRecursively(ar, &rows, iter, 0, version);
        }
        ar << rows;
    }

    template<class Archive>
    void saveRecursively(Archive &ar, std::vector<Row>* rows, Gtk::TreeIter iter, int depth, const unsigned int version) const {
        Row row;
        row.depth = depth;
        auto icon = iter->get_value(this->ModelColumns.ColumnIcon);
        row.image.data.reserve(icon->get_byte_length());
        std::copy(icon->get_pixels(), icon->get_pixels() + icon->get_byte_length(), std::back_inserter(row.image.data));
        // All images are squares, we don't need to save height, just one dimension is enough
        row.image.size = icon->get_width();
        row.image.rowstride = icon->get_rowstride();
        row.name = iter->get_value(this->ModelColumns.ColumnName);
        row.uri = iter->get_value(this->ModelColumns.ColumnURI);
        row.tooltip = iter->get_value(this->ModelColumns.ColumnTooltip);
        (*rows).push_back(row);

        if (!iter->children().empty()) {
            for (auto child = iter->children().begin(); child != iter->children().end(); child++) {
                this->saveRecursively(ar, rows, child, depth + 1, version);
            }
        }
    }

    template<class Archive>
    void load(Archive &ar, const unsigned int version) {
        std::vector<Row> rows;
        Gtk::TreeModel::iterator iter;
        // Using ChildRow will free TreeRow from memory and make it unaccessible
        std::vector<Gtk::TreeRow> parentRows;
        int lastDepth = -1;
        ar >> rows;

        double progressTotal = rows.size();
        double progressIteration = 0.0;
        this->mThreadProgress = 0.0;

        for(const auto &row : rows) {
            if (!this->mRefreshThreadRunning.load()) {
                pluginLog(LogLevel::Info, "Load canceled by user");
                this->mThreadProgress = 1.0;
                this->mBridge->notify();
                this->mRefreshLock = false;
                return;
            }

            if (parentRows.size() > 0 && row.depth <= lastDepth) {
                // We always add latest TreeRow, we should pop it if the depth is equal, 
                // if it's lesser, we should pop all higher TreeRows
                for (int i = lastDepth; i > row.depth - 1; i--) {
                    parentRows.pop_back();
                }
            }

            if (parentRows.size() > 0) {
                iter = this->append((parentRows[parentRows.size() - 1].children()));
            } else {
                iter = this->append();
            }

            Gtk::TreeRow treeRow = *iter;
            // We need to copy all the data on heap, as the original vector will get collected
            void* data = std::malloc(row.image.data.size());
            std::memcpy(data, row.image.data.data(), row.image.data.size());
            auto image = Gdk::Pixbuf::create_from_data(static_cast<const guint8*>(data), Gdk::COLORSPACE_RGB, false,
                8, row.image.size, row.image.size, row.image.rowstride);
            image->add_destroy_notify_callback(data, [](void* data) -> void* {
                std::free(data);
                return nullptr;
            });
            treeRow[this->ModelColumns.ColumnIcon] = image;
            treeRow[this->ModelColumns.ColumnName] = row.name;
            treeRow[this->ModelColumns.ColumnURI] = row.uri;
            treeRow[this->ModelColumns.ColumnTooltip] = row.tooltip;
            treeRow[this->ModelColumns.ColumnVisibility] = true;
            lastDepth = row.depth;
            
            parentRows.push_back(treeRow);

            this->mThreadProgress = progressIteration / progressTotal;
            this->mBridge->notify();
            progressIteration++;
        }

        pluginLog(LogLevel::Info, "Structure loaded");
        this->mRefreshLock = false;
        pluginLog(LogLevel::Info, "Notifying dispatcher - task done");
        this->mThreadProgress = 1.0;
        this->mBridge->notify();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};