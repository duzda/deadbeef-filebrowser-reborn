#include "treefilebrowser.hpp"

#include <boost/algorithm/string.hpp>

#include "filebrowser.hpp"
#include "utils.hpp"
#include "plugin.hpp"
#include "addressbox.hpp"

TreeFilebrowser::TreeFilebrowser() {
    this->set_column_types(mModelColumns);
    //this->treeStore = Gtk::TreeStore::create(mModelColumns);
    //mTreeView->set_model(this->treeStore);
}

Glib::RefPtr<TreeFilebrowser> TreeFilebrowser::create() {
    return Glib::RefPtr<TreeFilebrowser>(new TreeFilebrowser());
}

void TreeFilebrowser::setNeedleState(bool newState) {
    this->mIsNeedleSet = newState;
}

void TreeFilebrowser::initialize(Gtk::TreeView *treeview, Addressbox *addressbox) {
    this->mTreeView = treeview;
    this->mAddressbox = addressbox;
}

void TreeFilebrowser::setIconSize(uint newIconSize) {
    this->mIconSize = newIconSize;
}

void TreeFilebrowser::setTreeRoot(std::filesystem::path newDirectory) {
    if (refreshLock) {
        return;
    }
    this->mTreeDirectory = newDirectory;
    this->refreshTree();
}

void TreeFilebrowser::refreshTree() {
    if (refreshLock) {
        return;
    }
    refreshLock = true;
    mRefreshThread = new std::thread(&TreeFilebrowser::refreshThread, this);
}

void TreeFilebrowser::refreshThread() {
    mTreeView->unset_model();
    clear();
    pluginLog(DDB_LOG_LAYER_INFO, "Loading tree structure");
    auto filelist = Filebrowser::getFileList(mTreeDirectory, true, false);
    std::size_t count = filelist.size();
    progressCount = count;
    threadProgress = 0;
    progressIteration = 0;
    if (count > 0) {
        for (auto &entry : filelist) {
            if (!mRefreshThreadRun.load()) {
                pluginLog(DDB_LOG_LAYER_INFO, "Load canceled by user");
                threadProgress = 1;
                mAddressbox->notify();
                refreshLock = false;
                return;
            }
            threadProgress = progressIteration / progressCount;
            mAddressbox->notify();
            Glib::RefPtr<Gdk::Pixbuf> icon = Utils::getIcon(entry, mIconSize);
            Gtk::TreeModel::iterator iter = append();
            Gtk::TreeRow row = *iter;
            row[mModelColumns.mColumnIcon] = icon;
            row[mModelColumns.mColumnName] = entry.path().filename();
            row[mModelColumns.mColumnURI] = entry.path().string().erase(0, mTreeDirectory.string().size());
            row[mModelColumns.mColumnTooltip] = Utils::escapeTooltip(entry.path());
            row[mModelColumns.mColumnVisibility] = true;

            if (entry.is_directory() && !std::filesystem::is_empty(entry)) {
                this->findChildren(entry.path(), row.children());
            }
            progressIteration++;
        }
    } else {
        TreeFilebrowser::fillEmptyRow(this);
    }
    pluginLog(DDB_LOG_LAYER_INFO, "Structure loaded");
    //mTreeView->set_model(this);
    refreshLock = false;
    pluginLog(DDB_LOG_LAYER_INFO, "Notifying dispatcher - task done");
    threadProgress = 1;
    mAddressbox->notify();
}

void TreeFilebrowser::findChildren(std::filesystem::path path, Gtk::TreeNodeChildren child) {
    auto filelist = Filebrowser::getFileList(path, true, false);
    std::size_t count = filelist.size();
    //progressCount += count;
    if (count > 0) {
        for (auto &entry : filelist) {
            if (!mRefreshThreadRun.load()) {
                pluginLog(DDB_LOG_LAYER_INFO, "Child load canceled by user");
                return;
            }
            Glib::RefPtr<Gdk::Pixbuf> icon = Utils::getIcon(entry, mIconSize);
            Gtk::TreeModel::iterator iter = append(child);
            Gtk::TreeRow row = *iter;
            row[mModelColumns.mColumnIcon] = icon;
            row[mModelColumns.mColumnName] = entry.path().filename();
            row[mModelColumns.mColumnURI] = entry.path().string().erase(0, mTreeDirectory.string().size());
            row[mModelColumns.mColumnTooltip] = Utils::escapeTooltip(entry.path());
            row[mModelColumns.mColumnVisibility] = true;

            if (entry.is_directory() && !std::filesystem::is_empty(entry)) {
                this->findChildren(entry.path(), row.children());
            }
            //progressIteration++;
        }
    }
}

void TreeFilebrowser::stopThread() {
    if(mRefreshThread != NULL) {
        mRefreshThreadRun = false;
        mRefreshThread->join();
        mRefreshThread = NULL;
        mRefreshThreadRun = true;
    }
}

void TreeFilebrowser::getProgress(float* progress) const {
    if (refreshLock) {
        *progress = threadProgress;
    } else {
        *progress = 1;
    }
}

void TreeFilebrowser::checkEmptyRoot() {
    if (this->children().empty()) {
        g_idle_add((GSourceFunc)&TreeFilebrowser::fillEmptyRow, this);
    }
}

int TreeFilebrowser::fillEmptyRow(void* data) {
    auto self = static_cast<TreeFilebrowser*>(data);
    auto row = *(self->append());
    Glib::RefPtr<Gdk::Pixbuf> emptyIcon;
    row[self->mModelColumns.mColumnIcon] = emptyIcon;
    row[self->mModelColumns.mColumnURI] = "";
    row[self->mModelColumns.mColumnVisibility] = true;
    if (self->mIsNeedleSet) {
        row[self->mModelColumns.mColumnName] = "(EMPTY)";
        row[self->mModelColumns.mColumnTooltip] = "This directory is empty.";
    } else {
        row[self->mModelColumns.mColumnName] = "(NO RESULTS)";
        row[self->mModelColumns.mColumnTooltip] = "Try searching for something else.";
    }
    return 0;
}

TreeFilebrowser::~TreeFilebrowser() {
}