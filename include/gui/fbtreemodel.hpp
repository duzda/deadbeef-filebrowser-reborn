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
     * @return Float between 0 and 1, 1 when finished
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

private:
    friend boost::serialization::access;

    struct Image {
        std::vector<guint8> data;
        Gdk::Colorspace colorspace;
        gboolean has_alpha;
        int bits_per_sample;
        int width;
        int height;
        int rowstride;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & data & colorspace & has_alpha & bits_per_sample & width & height & rowstride;
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
        auto icon = iter->get_value(this->ModelColumns.ColumnIcon);
        row.depth = depth;
        row.image.data.reserve(icon->get_byte_length());
        pluginLog(LogLevel::Info, std::to_string(icon->get_byte_length()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_colorspace()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_has_alpha()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_bits_per_sample()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_width()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_height()));
        pluginLog(LogLevel::Info, std::to_string(icon->get_rowstride()));
        std::copy(icon->get_pixels(), icon->get_pixels() + icon->get_byte_length(), std::back_inserter(row.image.data));
        row.image.colorspace = icon->get_colorspace();
        row.image.has_alpha = icon->get_has_alpha();
        row.image.bits_per_sample = icon->get_bits_per_sample();
        row.image.width = icon->get_width();
        row.image.height = icon->get_height();
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
        for(const auto &row : rows) {
            pluginLog(LogLevel::Info, std::to_string(row.image.data.size()));
            pluginLog(LogLevel::Info, std::to_string(row.image.colorspace));
            pluginLog(LogLevel::Info, std::to_string(row.image.has_alpha));
            pluginLog(LogLevel::Info, std::to_string(row.image.bits_per_sample));
            pluginLog(LogLevel::Info, std::to_string(row.image.width));
            pluginLog(LogLevel::Info, std::to_string(row.image.height));
            pluginLog(LogLevel::Info, std::to_string(row.image.rowstride));
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
            treeRow[this->ModelColumns.ColumnIcon] = Gdk::Pixbuf::create_from_data(row.image.data.data(), row.image.colorspace, row.image.has_alpha, 
                row.image.bits_per_sample, row.image.width, row.image.height, row.image.rowstride);
            treeRow[this->ModelColumns.ColumnName] = row.name;
            treeRow[this->ModelColumns.ColumnURI] = row.uri;
            treeRow[this->ModelColumns.ColumnTooltip] = row.tooltip;
            treeRow[this->ModelColumns.ColumnVisibility] = true;
            lastDepth = row.depth;
            
            parentRows.push_back(treeRow);
        }

        this->mView->setModel();
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};