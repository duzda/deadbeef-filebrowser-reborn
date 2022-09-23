#include "treefilebrowser.hpp"

#include <boost/algorithm/string.hpp>

#include "filebrowser.hpp"
#include "utils.hpp"
#include "plugin.hpp"

TreeFilebrowser::TreeFilebrowser() {
    this->set_column_types(mModelColumns);
}

Glib::RefPtr<TreeFilebrowser> TreeFilebrowser::create() {
    return Glib::RefPtr<TreeFilebrowser>(new TreeFilebrowser());
}

void TreeFilebrowser::setNeedleState(bool newState) {
    this->mIsNeedleSet = newState;
}

void TreeFilebrowser::setTreeView(Gtk::TreeView *treeview) {
    this->mTreeView = treeview;
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
    this->mTreeView->freeze_notify();
    intptr_t tid = deadbeef->thread_start(&TreeFilebrowser::threadedRefreshTree, this);
    deadbeef->thread_detach(tid);
}

void TreeFilebrowser::threadedRefreshTree(void* data) {
    auto self = static_cast<TreeFilebrowser*>(data);
    self->refreshLock = true;
    g_idle_add((GSourceFunc)&TreeFilebrowser::clearRows, self);
    self->initialize(self->mTreeDirectory);
    self->checkEmptyRoot();
    self->mTreeView->thaw_notify();
    self->refreshLock = false;
}

void TreeFilebrowser::initialize(std::filesystem::path path, Gtk::TreeRow* parent) {
    auto filelist = Filebrowser::getFileList(path, true, false);
    if (!filelist.empty()) {
        for (auto &entry : filelist) {
            auto icon = Utils::getIcon(entry, mIconSize);
            // Possible memory leak?
            Gtk::TreeRow* row = new Gtk::TreeRow();
            structRowData* data = new structRowData();
            data->self = this;
            data->row = row;
            data->icon = icon;
            data->entry = entry;
            data->parent = parent;
            g_idle_add((GSourceFunc)&TreeFilebrowser::fillRow, data);

            if (entry.is_directory() && !std::filesystem::is_empty(entry)) {
                this->initialize(entry, row);
            }
        }
    } 
}

void TreeFilebrowser::checkEmptyRoot() {
    if (this->children().empty()) {
        g_idle_add((GSourceFunc)&TreeFilebrowser::fillEmptyRow, this);
    }
}

int TreeFilebrowser::fillRow(void* data) {
    auto rowData = static_cast<structRowData*>(data);
    if (rowData->parent) {
        *rowData->row = *(rowData->self->append(rowData->parent->children()));
    } else {
        *rowData->row = *(rowData->self->append());
    }
    (*rowData->row)[rowData->self->mModelColumns.mColumnIcon] = rowData->icon;
    (*rowData->row)[rowData->self->mModelColumns.mColumnName] = rowData->entry.path().filename();
    (*rowData->row)[rowData->self->mModelColumns.mColumnURI] = rowData->entry.path().string().erase(0, rowData->self->mTreeDirectory.string().size());
    (*rowData->row)[rowData->self->mModelColumns.mColumnTooltip] = Utils::escapeTooltip(rowData->entry.path());
    (*rowData->row)[rowData->self->mModelColumns.mColumnVisibility] = true;
    delete rowData;
    return 0;
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

int TreeFilebrowser::clearRows(void* data) {
    auto self = static_cast<TreeFilebrowser*>(data);
    self->clear();
    return 0;
}

TreeFilebrowser::~TreeFilebrowser() {
}