#include "container.hpp"

#include <filesystem>

#include "fbtreemodel.hpp"
#include "fbtreefilter.hpp"
#include "settings.hpp"
#include "plugin.hpp"
#include "filebrowser.hpp"
#include "utils.hpp"
#include "serializer.hpp"

using namespace GUI;

Container::Container() :
    mAddressbox(),
    mScrolledWindow()
{
    this->mModel = FBTreeModel::create();
    this->mFilter = FBTreeFilter::create(mModel);
    this->mSearchbar.initialize(mFilter.get());
    this->mBridge.initialize(&mAddressbox, &mSearchbar, &mView, mModel.get());
    this->mAddressbox.initialize(&mBridge, &mView, mFilter.get(), mModel.get());

    this->mView.initialize(mModel.get(), mFilter);
    this->mModel->initialize(&mBridge, &mView);
    this->mTreePopup.initialize(&mView, mModel.get(), mFilter.get(), &mAddressbox);

    this->mScrolledWindow.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
    this->mScrolledWindow.add(mView);

    this->pack_start(mSearchbar, false, true);
    this->pack_start(mAddressbox, false, true);
    this->pack_start(mScrolledWindow, true, true);

    std::filesystem::path defaultPath = Settings::getInstance().getDefaultPath();
    if (!std::filesystem::exists(defaultPath) || !std::filesystem::is_directory(defaultPath)) {
        return;
    }

    if (Cache::TreeModel::Serializer::shouldRecover(defaultPath)) {
        this->mModel->setTreeRoot(defaultPath);
        this->mModel->initialLoad();
    } else {
        this->mAddressbox.setAddress(defaultPath);
    }
}

Container::~Container() {
}