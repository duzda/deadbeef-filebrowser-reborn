#include "container.hpp"

#include "fbtreemodel.hpp"
#include "fbtreefilter.hpp"
#include "settings.hpp"
#include "plugin.hpp"
#include "filebrowser.hpp"
#include "utils.hpp"

using namespace GUI;

Container::Container() :
    mAddressbox(),
    mScrolledWindow()
{
    this->mModel = FBTreeModel::create();
    mModel->setIconSize(deadbeef->conf_get_int(FBR_ICON_SIZE, 32));

    this->mFilter = FBTreeFilter::create(mModel);
    this->mSearchbar.setTreeFilter(mFilter.get());
    this->mBridge.initialize(&mAddressbox, &mView, mModel.get());
    this->mAddressbox.initialize(&mBridge, &mView, mFilter.get(), mModel.get());

    this->mView.initialize(mModel.get(), mFilter);
    this->mModel->initialize(&mBridge, &mView);
    this->mTreePopup.initialize(&mView, mModel.get(), mFilter.get(), &mAddressbox);

    this->mScrolledWindow.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
    this->mScrolledWindow.add(mView);

    this->pack_start(mSearchbar, false, true);
    this->pack_start(mAddressbox, false, true);
    this->pack_start(mScrolledWindow, true, true);

    this->initialize();
}

void Container::initialize() {
    // Create autofilter
    Filebrowser::VALID_EXTENSIONS = Utils::createValidExtensions();
    std::string defaultDir = Glib::get_user_special_dir(Glib::UserDirectory::USER_DIRECTORY_MUSIC);

    // Set addressbar
    deadbeef->conf_lock();
    auto address = deadbeef->conf_get_str_fast(FBR_DEFAULT_PATH, defaultDir.c_str());
    deadbeef->conf_unlock();
    mAddressbox.setAddress(address);
}

Container::~Container() {
}