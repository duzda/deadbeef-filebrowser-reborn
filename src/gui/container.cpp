#include "container.hpp"

#include "settings.hpp"
#include "plugin.hpp"
#include "filebrowser.hpp"
#include "utils.hpp"

Container::Container() :
    mAddressbox(),
    mScrolledWindow()
{
    this->mFilebrowserModel = FilebrowserModel::create();
    mFilebrowserModel->setIconSize(deadbeef->conf_get_int(FBR_ICON_SIZE, 32));

    this->mFilebrowserFilter = FilebrowserFilter::create(mFilebrowserModel);
    this->mFilebrowserFilter->setModel(mFilebrowserModel.get());
    this->mSearchbar.setTreeModelFilter(this->mFilebrowserFilter.get());
    this->mAddressbox.initialize(&mTreeView, mFilebrowserFilter, this->mFilebrowserModel.get());

    this->mTreeView.set_model(mFilebrowserFilter);
    this->mTreeView.initialize(mFilebrowserModel.get());
    this->mFilebrowserModel->initialize(&this->mTreeView, &this->mAddressbox);
    this->mTreePopup.initialize(&this->mTreeView, this->mFilebrowserModel.get(), this->mFilebrowserFilter.get(), &this->mAddressbox);

    this->mScrolledWindow.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
    this->mScrolledWindow.add(mTreeView);

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