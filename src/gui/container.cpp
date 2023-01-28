#include "container.hpp"

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

    if (Cache::TreeModel::Serializer::exists()) {
        this->mModel->setTreeRoot(Settings::getInstance().getDefaultPath());
        Cache::TreeModel::Serializer::load(this->mModel.get());
    } else {
        this->mAddressbox.setAddress(Settings::getInstance().getDefaultPath());
    }
}

Container::~Container() {
}