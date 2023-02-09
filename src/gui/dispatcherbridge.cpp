#include "dispatcherbridge.hpp"

#include "addressbox.hpp"
#include "searchbar.hpp"
#include "fbtreeview.hpp"
#include "fbtreemodel.hpp"
#include "plugin.hpp"

using namespace GUI;

DispatcherBridge::DispatcherBridge() {
    mDispatcher.connect(sigc::mem_fun(*this, &DispatcherBridge::updateProgressState));
}

void DispatcherBridge::initialize(Addressbox* addressbox, Searchbar* searchbar, FBTreeView* view, FBTreeModel* model) {
    this->mAddressbox = addressbox;
    this->mSearchbar = searchbar;
    this->mView = view;
    this->mModel = model;
}

bool DispatcherBridge::inProgress() const {
    return this->mInProgress;
}

void DispatcherBridge::onStart() {
    pluginLog(LogLevel::Info, "Changing buttons, refesh has began.");
    this->mAddressbox->setState(false);
    this->mSearchbar->set_sensitive(false);
    this->mInProgress = true;
}

void DispatcherBridge::notify() {
    mDispatcher.emit();
}

void DispatcherBridge::updateProgressState() {
    if (this->mInProgress) {
        double progress = mModel->getProgress();
        if (progress == 1.0) {
            pluginLog(LogLevel::Info, "Thread reported progress done");
            this->mInProgress = false;
            this->mAddressbox->setState(true);
            this->mSearchbar->set_sensitive(true);
            this->mAddressbox->setProgress(0.0);
            this->mView->setModel();
        } else {
            this->mAddressbox->setProgress(progress);
        }
    }
}

DispatcherBridge::~DispatcherBridge() {
}