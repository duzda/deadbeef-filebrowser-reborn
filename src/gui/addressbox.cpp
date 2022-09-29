#include "addressbox.hpp"

#include "filesystem"

#include "plugin.hpp"
#include "settings.hpp"
#include "utils.hpp"

Addressbox::Addressbox() :
mGoButton() {
    this->mGoButton.signal_clicked().connect(sigc::mem_fun(*this, &Addressbox::on_go_button_click));
    this->mGoButton.set_margin_top(1);
    this->mGoButton.set_margin_bottom(1);
    this->mGoButton.set_margin_left(1);
    this->mGoButton.set_margin_right(1);
    this->mGoButton.set_image(*Gtk::manage(new Gtk::Image(Utils::getIconByName("go-next", 16))));
    this->pack_start(this->mAddressBar, true, true);
    this->pack_start(this->mGoButton, false, true);

    // Connect the handler to the dispatcher.
    mDispatcher.connect(sigc::mem_fun(*this, &Addressbox::onNotify));
}

void Addressbox::initialize(Gtk::TreeView *treeview, Glib::RefPtr<FilebrowserFilter> filter) {
    this->mTreeView = treeview;
    this->mFilebrowserFilter = filter;
}

void Addressbox::setTreeFilebrowser(TreeFilebrowser* newTreeFilebrowser) {
    this->mTreeFilebrowser = newTreeFilebrowser;
}

void Addressbox::setAddress(std::string newAddress) {
    this->mAddressBar.set_text(newAddress);
    this->on_go_button_click();
}

void Addressbox::updateProgress() {
    float progress;
    mTreeFilebrowser->getProgress(&progress);
    if (progress == 1) {
        pluginLog(DDB_LOG_LAYER_INFO, "Thread reported progress done");
        inProgress = false;
        if (inProgress == false) {
            pluginLog(DDB_LOG_LAYER_INFO, "Thread just finished, changing UI");
            this->mAddressBar.set_sensitive(true);
            this->mGoButton.set_image(*Gtk::manage(new Gtk::Image(Utils::getIconByName("go-next", 16))));
            this->mAddressBar.set_progress_fraction(0);
            this->mTreeView->set_model(this->mFilebrowserFilter);
        }
    } else {
        if (inProgress == false) {
            pluginLog(DDB_LOG_LAYER_INFO, "Thread just started running, changing buttons");
            //this->mTreeView->unset_model();
            this->mAddressBar.set_sensitive(false);
            this->mGoButton.set_image(*Gtk::manage(new Gtk::Image(Utils::getIconByName("process-stop", 16))));
            inProgress = true;
        }
        this->mAddressBar.set_progress_fraction(progress);
    }
}

void Addressbox::setProgressBar(ProgressBarData* data) {
    data->bar->set_progress_fraction(data->progress);
}

void Addressbox::notify() {
    mDispatcher.emit();
}

void Addressbox::onNotify() {
    this->updateProgress();
}

std::string Addressbox::getAddress() {
    return this->mAddress;
}

void Addressbox::on_go_button_click() {
    if (inProgress) {
        pluginLog(DDB_LOG_LAYER_INFO, "Still in progress");
        this->mTreeFilebrowser->stopThread();
        return;
    }
    this->mAddress = this->mAddressBar.get_text();
    this->mAddress = this->makeValidPath(this->mAddress);
    deadbeef->conf_set_str(FBR_DEFAULT_PATH, this->mAddress.c_str());
    this->mAddressBar.set_text(this->mAddress);
    if (std::filesystem::exists(this->mAddress) && std::filesystem::is_directory(this->mAddress)) {
        this->mTreeFilebrowser->setTreeRoot(this->mAddress);
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