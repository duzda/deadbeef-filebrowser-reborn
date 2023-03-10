#include "addressbox.hpp"

#include <filesystem>

#include "dispatcherbridge.hpp"
#include "fbtreemodel.hpp"
#include "fbtreeview.hpp"
#include "plugin.hpp"
#include "settings.hpp"
#include "iconutils.hpp"

using namespace GUI;

Addressbox::Addressbox() :
mGoButton() {
    this->mGoButton.signal_clicked().connect(sigc::mem_fun(*this, &Addressbox::on_go_button_click));
    this->mGoButton.set_margin_top(1);
    this->mGoButton.set_margin_bottom(1);
    this->mGoButton.set_margin_left(1);
    this->mGoButton.set_margin_right(1);
    this->mGoButton.set_image(*Gtk::manage(new Gtk::Image(IconUtils::getIconByName("go-next", ICON_SIZE))));

    this->mAddressBar.signal_activate().connect(sigc::mem_fun(*this, &Addressbox::on_go_button_click));

    this->pack_start(this->mAddressBar, true, true);
    this->pack_start(this->mGoButton, false, true);
}

void Addressbox::initialize(DispatcherBridge* bridge, FBTreeView* view, FBTreeFilter* filter, FBTreeModel* model) {
    this->mBridge = bridge;
    this->mView = view;
    this->mFilter = filter;
    this->mModel = model;
    this->mAddressBar.set_text(Settings::getInstance().getDefaultPath());
}

void Addressbox::setAddress(std::string address) {
    this->mAddressBar.set_text(address);
    this->on_go_button_click();
}

std::string Addressbox::getAddress() const {
    return this->mAddress;
}

void Addressbox::setProgress(double progress) {
    if (this->mAddressBar.get_progress_fraction() != progress) {
        this->mAddressBar.set_progress_fraction(progress);
    }
}

void Addressbox::setState(bool running) {
    this->mAddressBar.set_sensitive(running);
    this->mGoButton.set_image(*Gtk::manage(new Gtk::Image(IconUtils::getIconByName(running ? "go-next" : "process-stop", ICON_SIZE))));
}

void Addressbox::on_go_button_click() {
    if (mBridge->inProgress()) {
        pluginLog(LogLevel::Info, "Still in progress");
        this->mModel->stopThread();
        return;
    }
    auto address = this->mAddressBar.get_text();
    address = this->makeValidPath(address);
    deadbeef->conf_set_str(FBR_DEFAULT_PATH, address.c_str());
    this->mAddressBar.set_text(address);
    if (std::filesystem::exists((std::string)address) && std::filesystem::is_directory((std::string)address)) {
        this->mAddress = address;
        this->mModel->setTreeRoot((std::string)address);
        this->mModel->refreshTree();
    }
}

std::string Addressbox::makeValidPath(std::string path) {
    uint count = 0;
    for (uint i = path.size() - 1; i >= 0; i--) {
        if (path[i] == '/') {
            count++;
        } else {
            break;
        }
    }

    path.resize(path.size() - count);
    return path;
}

Addressbox::~Addressbox() {
}