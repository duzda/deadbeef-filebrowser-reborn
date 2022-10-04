#include "searchbar.hpp"

#include "fbtreefilter.hpp"
#include "utils.hpp"

using namespace GUI;

Searchbar::Searchbar() {
    auto primaryIcon = this->property_primary_icon_pixbuf();
    primaryIcon = Utils::getIconByName("system-search", 16);
}

void Searchbar::setTreeFilter(FBTreeFilter* filter) {
    this->mFilter = filter;
    this->signal_changed().connect(sigc::mem_fun(this, &Searchbar::on_change));
}

void Searchbar::on_change() {
    mTimer.disconnect();
    mTimer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Searchbar::on_timeout), timeoutValue);
}

bool Searchbar::on_timeout() {
    this->mFilter->setNeedle(this->get_text());
    return false;
}

Searchbar::~Searchbar() {
}