#include "fbtreemodel.hpp"

#include <boost/algorithm/string.hpp>

#include "dispatcherbridge.hpp"
#include "fbtreeview.hpp"
#include "filebrowser.hpp"
#include "utils.hpp"
#include "plugin.hpp"
#include "addressbox.hpp"
#include "settings.hpp"

using namespace GUI;

FBTreeModel::FBTreeModel() {
    this->set_column_types(ModelColumns);
}

Glib::RefPtr<FBTreeModel> FBTreeModel::create() {
    return Glib::RefPtr<FBTreeModel>(new FBTreeModel());
}

void FBTreeModel::setNeedleState(bool newState) {
    this->mIsNeedleSet = newState;
}

void FBTreeModel::initialize(DispatcherBridge* bridge, FBTreeView* view) {
    this->mBridge = bridge;
    this->mView = view;
}

float FBTreeModel::getProgress() {
    return (this->mRefreshLock ? (float)this->mThreadProgress : 1);
}

void FBTreeModel::setTreeRoot(std::filesystem::path newDirectory) {
    if (this->mRefreshLock) {
        return;
    }
    this->mTreeDirectory = newDirectory;
    this->refreshTree();
}

void FBTreeModel::refreshTree() {
    if (this->mRefreshLock) {
        return;
    }
    this->mRefreshLock = true;
    this->mView->unset_model();
    this->mRefreshThread = new std::thread(&FBTreeModel::refreshThread, this);
}

void FBTreeModel::refreshThread() {
    this->clear();
    pluginLog(DDB_LOG_LAYER_INFO, "Loading tree structure");
    auto filelist = Filebrowser::getFileList(mTreeDirectory, true, false);
    float progressTotal = filelist.size();
    float progressIteration = 0;
    this->mThreadProgress = 0;
    if (progressTotal > 0) {
        for (auto &entry : filelist) {
            if (!this->mRefreshThreadRunning.load()) {
                pluginLog(DDB_LOG_LAYER_INFO, "Load canceled by user");
                this->mThreadProgress = 1;
                this->mBridge->notify();
                this->mRefreshLock = false;
                return;
            }
            this->mThreadProgress = progressIteration / progressTotal;
            this->mBridge->notify();
            this->fillRow(entry);
            progressIteration++;
        }
    } else {
        this->fillEmptyRow();
    }
    pluginLog(DDB_LOG_LAYER_INFO, "Structure loaded");
    this->mRefreshLock = false;
    pluginLog(DDB_LOG_LAYER_INFO, "Notifying dispatcher - task done");
    this->mThreadProgress = 1;
    this->mBridge->notify();
}

void FBTreeModel::stopThread() {
    if(this->mRefreshThread != NULL) {
        this->mRefreshThreadRunning = false;
        this->mRefreshThread->join();
        this->mRefreshThread = NULL;
        this->mRefreshThreadRunning = true;
    }
}

void FBTreeModel::fillRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent) {
    Glib::RefPtr<Gdk::Pixbuf> icon = Utils::getIcon(entry, Settings::getInstance().getIconSize());
    Gtk::TreeModel::iterator iter;
    if (parent) {
        iter = append((*parent));
    } else {
        iter = append();
    }
    Gtk::TreeRow row = *iter;
    row[ModelColumns.ColumnIcon] = icon;
    row[ModelColumns.ColumnName] = entry.path().filename();
    row[ModelColumns.ColumnURI] = entry.path().string().erase(0, mTreeDirectory.string().size());
    row[ModelColumns.ColumnTooltip] = Utils::escapeTooltip(entry.path());
    row[ModelColumns.ColumnVisibility] = true;

    if (entry.is_directory() && !std::filesystem::is_empty(entry)) {
        this->fillChildrenRow(entry, &row.children());
    }
}

void FBTreeModel::fillChildrenRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent) {
    auto filelist = Filebrowser::getFileList(entry.path(), true, false);
    std::size_t count = filelist.size();
    if (count > 0) {
        for (auto &entry : filelist) {
            if (!mRefreshThreadRunning.load()) {
                pluginLog(DDB_LOG_LAYER_INFO, "Child load canceled by user");
                return;
            }
            this->fillRow(entry, parent);
        }
    }
}

void FBTreeModel::fillEmptyRow() {
    auto row = *(this->append());
    Glib::RefPtr<Gdk::Pixbuf> emptyIcon;
    row[this->ModelColumns.ColumnIcon] = emptyIcon;
    row[this->ModelColumns.ColumnURI] = "";
    row[this->ModelColumns.ColumnVisibility] = true;
    if (this->mIsNeedleSet) {
        row[this->ModelColumns.ColumnName] = "(EMPTY)";
        row[this->ModelColumns.ColumnTooltip] = "This directory is empty.";
    } else {
        row[this->ModelColumns.ColumnName] = "(NO RESULTS)";
        row[this->ModelColumns.ColumnTooltip] = "Try searching for something else.";
    }
}

FBTreeModel::~FBTreeModel() {
}