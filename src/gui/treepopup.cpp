#include "treepopup.hpp"

#include <filesystem>

#include "plugin.hpp"
#include "settings.hpp"

TreePopup::TreePopup() {
    this->mMenu = Gio::Menu::create();

    this->mMenu->append("_Add to current playlist", "popup.add");
    this->mMenu->append("_Replace current playlist", "popup.replace");
    this->mMenu->append("_Open containing folder", "popup.open");
    this->mMenu->append("_Enter directory", "popup.enter");
    this->mMenu->append("Go _up", "popup.up");
    this->mMenu->append("_Refresh", "popup.refresh");

    this->mActionGroup = Gio::SimpleActionGroup::create();

    this->mActionGroup->add_action("add", sigc::mem_fun(*this, &TreePopup::popup_add));
    this->mActionGroup->add_action("replace", sigc::mem_fun(*this, &TreePopup::popup_replace));
    this->mActionGroup->add_action("open", sigc::mem_fun(*this, &TreePopup::popup_open_folder));
    this->mActionGroup->add_action("enter", sigc::mem_fun(*this, &TreePopup::popup_enter_directory));
    this->mActionGroup->add_action("up", sigc::mem_fun(*this, &TreePopup::popup_go_up));
    this->mActionGroup->add_action("refresh", sigc::mem_fun(*this, &TreePopup::popup_refresh));

    this->insert_action_group("popup", this->mActionGroup);
    this->bind_model(mMenu, true);
}

void TreePopup::initialize(Gtk::TreeView* treeview, TreeFilebrowser* treefb, FilebrowserFilter* filter, Addressbox* addressbox) {
    this->mTreeView = treeview;
    this->mTreeFilebrowser = treefb;
    this->mFilter = filter;
    this->mAddressbox = addressbox;

    treeview->signal_button_press_event().connect(sigc::mem_fun(*this, &TreePopup::on_click), false);

    this->show_all();
}

bool TreePopup::on_click(GdkEventButton* event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        this->popup_at_pointer(NULL);
    } else if (event->type == GDK_2BUTTON_PRESS && event->button == 1) {
        if (deadbeef->conf_get_int(FBR_DOUBLECLICK_REPLACE, 0)) {
            popup_replace();
        } else {
            popup_add();
        }
    }
    return false;
}

void TreePopup::popup_add() {
    mDataHolder.address = mAddressbox->getAddress();
    mDataHolder.uris = this->getSelectedURIs();
    mDataHolder.replace = false;
    intptr_t tid = deadbeef->thread_start(TreePopup::threadedAddToPlaylist, static_cast<void*>(&mDataHolder));
    deadbeef->thread_detach(tid);
}

void TreePopup::popup_replace() {
    mDataHolder.address = mAddressbox->getAddress();
    mDataHolder.uris = this->getSelectedURIs();
    mDataHolder.replace = true;
    intptr_t tid = deadbeef->thread_start(TreePopup::threadedAddToPlaylist, static_cast<void*>(&mDataHolder));
    deadbeef->thread_detach(tid);
}

void TreePopup::threadedAddToPlaylist(void* ctx) {
    auto holder = (TreePopup::structAddToPlaylist*)(ctx);
    TreePopup::addToPlaylist(holder->uris, holder->address, holder->replace);
}

void TreePopup::addToPlaylist(std::vector<std::string> uris, std::string address, bool replace) {
    ddb_playlist_t* plt = deadbeef->plt_get_curr();

    if (replace) {
        deadbeef->plt_select_all(plt);
        deadbeef->plt_delete_selected(plt);
    }

    for (auto &uri : uris) {
        std::string realURI = address + uri;

        deadbeef->plt_add_files_begin(plt, 0);

        if (std::filesystem::is_directory(realURI)) {
            deadbeef->plt_add_dir2(0, plt, realURI.c_str(), NULL, NULL);
        } else {
            deadbeef->plt_add_file2(0, plt, realURI.c_str(), NULL, NULL);
        }

        deadbeef->plt_add_files_end(plt, 0);
        deadbeef->plt_modified(plt);
        deadbeef->plt_save_config(plt);
        deadbeef->conf_save();
    }

    if (replace) {
        deadbeef->sendmessage(DB_EV_PLAY_NUM, 0, 0, 0);
    } else {
        if (deadbeef->playback_get_pos() == 0.0) {
            deadbeef->sendmessage(DB_EV_NEXT, 0, 0, 0);
        }
    }
}

void TreePopup::popup_open_folder() {
    if (hasSelected()) {
        GError *error = NULL;
        std::filesystem::path path = std::filesystem::path(mAddressbox->getAddress() + this->getSelectedURI());
        std::filesystem::directory_entry entry = std::filesystem::directory_entry(path);
        if (!entry.is_directory()) {
            path = entry.path().parent_path();
        }
        auto URI = "file:///" + path.string();
        if (!g_app_info_launch_default_for_uri(URI.c_str(), NULL, &error)) {
            g_warning("Failed to oepn uri: %s", error->message);
        }
    }
}

void TreePopup::popup_enter_directory() {
    if (hasSelected()) {
        this->mTreeFilebrowser->stopThread();
        auto URI = mAddressbox->getAddress() + this->getSelectedURI();
        mAddressbox->setAddress(URI);
    }
}

void TreePopup::popup_go_up() {
    auto URI = std::filesystem::path(mAddressbox->getAddress());
    if (URI.has_parent_path()) {
        this->mTreeFilebrowser->stopThread();
        mAddressbox->setAddress(URI.parent_path());
    }
}

void TreePopup::popup_refresh() {
    mTreeFilebrowser->refreshTree();
}

std::string TreePopup::getSelectedURI() {
    return mFilter->get_iter(mTreeView->get_selection()->get_selected_rows()[0])->get_value(mTreeFilebrowser->mModelColumns.mColumnURI);
}

std::vector<std::string> TreePopup::getSelectedURIs() {
    std::vector<std::string> uris;
    auto rows = mTreeView->get_selection()->get_selected_rows();
    for (auto &row : rows) {
        uris.push_back(mFilter->get_iter(row)->get_value(mTreeFilebrowser->mModelColumns.mColumnURI));
    }

    return uris;
}

bool TreePopup::hasSelected() {
    return mTreeView->get_selection()->get_selected_rows().size() != 0;
}

TreePopup::~TreePopup() {
}