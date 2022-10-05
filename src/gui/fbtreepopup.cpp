#include "fbtreepopup.hpp"

#include <filesystem>

#include "fbtreeview.hpp"
#include "fbtreemodel.hpp"
#include "addressbox.hpp"
#include "plugin.hpp"
#include "settings.hpp"

using namespace GUI;

FBTreePopup::FBTreePopup() {
    this->mMenu = Gio::Menu::create();

    this->mMenu->append("_Add to current playlist", "popup.add");
    this->mMenu->append("_Replace current playlist", "popup.replace");
    this->mMenu->append("_Open containing folder", "popup.open");
    this->mMenu->append("_Enter directory", "popup.enter");
    this->mMenu->append("Go _up", "popup.up");
    this->mMenu->append("_Refresh", "popup.refresh");

    this->mActionGroup = Gio::SimpleActionGroup::create();

    this->mActionGroup->add_action("add", sigc::mem_fun(*this, &FBTreePopup::popup_add));
    this->mActionGroup->add_action("replace", sigc::mem_fun(*this, &FBTreePopup::popup_replace));
    this->mActionGroup->add_action("open", sigc::mem_fun(*this, &FBTreePopup::popup_open_folder));
    this->mActionGroup->add_action("enter", sigc::mem_fun(*this, &FBTreePopup::popup_enter_directory));
    this->mActionGroup->add_action("up", sigc::mem_fun(*this, &FBTreePopup::popup_go_up));
    this->mActionGroup->add_action("refresh", sigc::mem_fun(*this, &FBTreePopup::popup_refresh));

    this->insert_action_group("popup", this->mActionGroup);
    this->bind_model(mMenu, true);
}

void FBTreePopup::initialize(FBTreeView* view, FBTreeModel* model, FBTreeFilter* filter, Addressbox* addressbox) {
    this->mView = view;
    this->mModel = model;
    this->mFilter = filter;
    this->mAddressbox = addressbox;

    this->mView->signal_button_press_event().connect(sigc::mem_fun(*this, &FBTreePopup::on_click), false);

    this->show_all();
}

bool FBTreePopup::on_click(GdkEventButton* event) {
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

void FBTreePopup::popup_add() {
    mDataHolder.address = mAddressbox->getAddress();
    mDataHolder.uris = mView->getSelectedURIs();
    mDataHolder.replace = false;
    intptr_t tid = deadbeef->thread_start(FBTreePopup::threadedAddToPlaylist, static_cast<void*>(&mDataHolder));
    deadbeef->thread_detach(tid);
}

void FBTreePopup::popup_replace() {
    mDataHolder.address = mAddressbox->getAddress();
    mDataHolder.uris = mView->getSelectedURIs();
    mDataHolder.replace = true;
    intptr_t tid = deadbeef->thread_start(FBTreePopup::threadedAddToPlaylist, static_cast<void*>(&mDataHolder));
    deadbeef->thread_detach(tid);
}

void FBTreePopup::threadedAddToPlaylist(void* ctx) {
    auto holder = (FBTreePopup::structAddToPlaylist*)(ctx);
    FBTreePopup::addToPlaylist(holder->uris, holder->address, holder->replace);
}

void FBTreePopup::addToPlaylist(std::vector<std::string> uris, std::string address, bool replace) {
    ddb_playlist_t* plt = deadbeef->plt_get_curr();

    int itemCount = deadbeef->plt_get_item_count(plt, PL_MAIN);
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
    }

    if (replace) {
        deadbeef->sendmessage(DB_EV_PLAY_NUM, 0, 0, 0);
    } else {
        auto current = deadbeef->streamer_get_playing_track();
        if (!current) {
            if (itemCount == 0) {
                deadbeef->sendmessage(DB_EV_PLAY_NUM, 0, 0, 0);
            } else {
                if (deadbeef->plt_get_item_count(plt, PL_MAIN) > itemCount) {
                    deadbeef->sendmessage(DB_EV_PLAY_NUM, 0, itemCount, 0);
                }
            }
        }
    }

    deadbeef->plt_unref(plt);
}

void FBTreePopup::popup_open_folder() {
    if (mView->hasSelected()) {
        GError *error = NULL;
        std::filesystem::path path = std::filesystem::path(mAddressbox->getAddress() + mView->getSelectedURI());
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

void FBTreePopup::popup_enter_directory() {
    if (mView->hasSelected()) {
        this->mModel->stopThread();
        auto URI = mAddressbox->getAddress() + mView->getSelectedURI();
        this->mAddressbox->setAddress(URI);
    }
}

void FBTreePopup::popup_go_up() {
    auto URI = std::filesystem::path(mAddressbox->getAddress());
    if (URI.has_parent_path()) {
        this->mModel->stopThread();
        this->mAddressbox->setAddress(URI.parent_path());
    }
}

void FBTreePopup::popup_refresh() {
    this->mModel->refreshTree();
}

FBTreePopup::~FBTreePopup() {
}