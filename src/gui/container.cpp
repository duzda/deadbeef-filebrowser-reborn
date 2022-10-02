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
    this->buildTreeview();

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

void Container::buildTreeview() {
    this->mTreeView.set_model(mFilebrowserFilter);
    this->mFilebrowserModel->initialize(&this->mTreeView, &this->mAddressbox);

    this->mTreeView.append_column("Icon", this->mFilebrowserModel->mModelColumns.mColumnIcon);
    this->mTreeView.append_column("Name", this->mFilebrowserModel->mModelColumns.mColumnName);

    for (uint i = 0; i < 2; i++) {
        this->mTreeView.get_column(i)->set_spacing(0);
        this->mTreeView.get_column(i)->set_sizing(Gtk::TreeViewColumnSizing::TREE_VIEW_COLUMN_AUTOSIZE);
    }

    this->mTreeView.set_enable_search(true);
    this->mTreeView.set_search_column(1);
    this->mTreeView.set_expander_column(*this->mTreeView.get_column(0));
    this->mTreeView.set_show_expanders(true);
    this->mTreeView.set_enable_tree_lines(true);
    this->mTreeView.set_headers_visible(false);
    this->mTreeView.get_selection()->set_mode(Gtk::SelectionMode::SELECTION_MULTIPLE);
    this->mTreeView.set_has_tooltip(true);
    this->mTreeView.set_tooltip_column(3);

    this->mTreePopup.initialize(&this->mTreeView, this->mFilebrowserModel.get(), this->mFilebrowserFilter.get(), &this->mAddressbox);
}

Container::~Container() {
}