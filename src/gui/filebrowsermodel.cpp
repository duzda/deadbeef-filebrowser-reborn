#include "filebrowsermodel.hpp"

#include <boost/algorithm/string.hpp>

#include "filebrowser.hpp"
#include "utils.hpp"
#include "plugin.hpp"
#include "addressbox.hpp"

FilebrowserModel::FilebrowserModel() {
    this->set_column_types(mModelColumns);
}

Glib::RefPtr<FilebrowserModel> FilebrowserModel::create() {
    return Glib::RefPtr<FilebrowserModel>(new FilebrowserModel());
}

void FilebrowserModel::setNeedleState(bool newState) {
    this->mIsNeedleSet = newState;
}

void FilebrowserModel::initialize(Gtk::TreeView* treeview, Addressbox* addressbox) {
    this->mTreeView = treeview;
    this->mAddressbox = addressbox;
}

void FilebrowserModel::setIconSize(uint newIconSize) {
    this->mIconSize = newIconSize;
}

float FilebrowserModel::getProgress() {
    return (this->mRefreshLock ? (float)this->mThreadProgress : 1);
}

void FilebrowserModel::setTreeRoot(std::filesystem::path newDirectory) {
    if (this->mRefreshLock) {
        return;
    }
    this->mTreeDirectory = newDirectory;
    this->refreshTree();
}

void FilebrowserModel::refreshTree() {
    if (this->mRefreshLock) {
        return;
    }
    this->mRefreshLock = true;
    this->mTreeView->unset_model();
    this->mRefreshThread = new std::thread(&FilebrowserModel::refreshThread, this);
}

void FilebrowserModel::refreshThread() {
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
                this->mAddressbox->notify();
                this->mRefreshLock = false;
                return;
            }
            this->mThreadProgress = progressIteration / progressTotal;
            this->mAddressbox->notify();
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
    this->mAddressbox->notify();
}

void FilebrowserModel::stopThread() {
    if(this->mRefreshThread != NULL) {
        this->mRefreshThreadRunning = false;
        this->mRefreshThread->join();
        this->mRefreshThread = NULL;
        this->mRefreshThreadRunning = true;
    }
}

void FilebrowserModel::fillRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent) {
    Glib::RefPtr<Gdk::Pixbuf> icon = Utils::getIcon(entry, mIconSize);
    Gtk::TreeModel::iterator iter;
    if (parent) {
        iter = append((*parent));
    } else {
        iter = append();
    }
    Gtk::TreeRow row = *iter;
    row[mModelColumns.mColumnIcon] = icon;
    row[mModelColumns.mColumnName] = entry.path().filename();
    row[mModelColumns.mColumnURI] = entry.path().string().erase(0, mTreeDirectory.string().size());
    row[mModelColumns.mColumnTooltip] = Utils::escapeTooltip(entry.path());
    row[mModelColumns.mColumnVisibility] = true;

    if (entry.is_directory() && !std::filesystem::is_empty(entry)) {
        this->fillChildrenRow(entry, &row.children());
    }
}

void FilebrowserModel::fillChildrenRow(std::filesystem::directory_entry entry, const Gtk::TreeNodeChildren* parent) {
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

void FilebrowserModel::fillEmptyRow() {
    auto row = *(this->append());
    Glib::RefPtr<Gdk::Pixbuf> emptyIcon;
    row[this->mModelColumns.mColumnIcon] = emptyIcon;
    row[this->mModelColumns.mColumnURI] = "";
    row[this->mModelColumns.mColumnVisibility] = true;
    if (this->mIsNeedleSet) {
        row[this->mModelColumns.mColumnName] = "(EMPTY)";
        row[this->mModelColumns.mColumnTooltip] = "This directory is empty.";
    } else {
        row[this->mModelColumns.mColumnName] = "(NO RESULTS)";
        row[this->mModelColumns.mColumnTooltip] = "Try searching for something else.";
    }
}

FilebrowserModel::~FilebrowserModel() {
}