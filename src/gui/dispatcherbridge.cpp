#include "dispatcherbridge.hpp"

#include "addressbox.hpp"
#include "fbtreeview.hpp"
#include "fbtreemodel.hpp"
#include "plugin.hpp"

using namespace GUI;

DispatcherBridge::DispatcherBridge() {
    mDispatcher.connect(sigc::mem_fun(*this, &DispatcherBridge::onNotify));
}

void DispatcherBridge::initialize(Addressbox* addressbox, FBTreeView* view, FBTreeModel* model) {
    this->mAddressbox = addressbox;
    this->mView = view;
    this->mModel = model;
}

bool DispatcherBridge::inProgress() {
    return this->mInProgress;
}

void DispatcherBridge::notify() {
    mDispatcher.emit();
}

void DispatcherBridge::onNotify() {
    this->updateProgressState();
}

void DispatcherBridge::updateProgressState() {
    double progress = mModel->getProgress();
    if (progress == 1) {
        pluginLog(LogLevel::Info, "Thread reported progress done");
        this->mInProgress = false;
        this->mAddressbox->setState(true);
        this->mAddressbox->setProgress(0.0);
        this->mView->setModel();
    } else {
        if (!this->mInProgress) {
            pluginLog(LogLevel::Info, "Thread just started running, changing buttons");
            this->mAddressbox->setState(false);
            this->mInProgress = true;
        }
        this->mAddressbox->setProgress(progress);
    }
}

DispatcherBridge::~DispatcherBridge() {
}