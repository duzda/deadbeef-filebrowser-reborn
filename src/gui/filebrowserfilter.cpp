#include "filebrowserfilter.hpp"

#include <boost/algorithm/string.hpp>

FilebrowserFilter::FilebrowserFilter(Glib::RefPtr<Gtk::TreeModel> model) :
Gtk::TreeModelFilter(model) {
}

Glib::RefPtr<FilebrowserFilter> FilebrowserFilter::create(Glib::RefPtr<TreeFilebrowser> treefb) {
    return Glib::RefPtr<FilebrowserFilter>(new FilebrowserFilter(treefb));
}

void FilebrowserFilter::setNeedle(std::string newNeedle) {
    this->mNeedle = boost::to_lower_copy(newNeedle);
    this->mTreeFilebrowser->setNeedleState(this->mNeedle.empty());
    this->refilter();
}

void FilebrowserFilter::setModel(TreeFilebrowser* treefb) {
    this->mTreeFilebrowser = treefb;
    this->set_visible_func(mem_fun(*this, &FilebrowserFilter::filter_func));
}

bool FilebrowserFilter::filter_func(Gtk::TreeIter iter) {
    if (!iter->children().empty()) {
        // Check children, this is not very efficient, but the only way I could think of
        auto child = iter->children().end();
        do {
            child--;
            if (this->filter_func(child)) {
                return true;
            }
        } while (child != iter->children().begin());
        return false;
    }

    return (this->mNeedle.empty() || boost::algorithm::contains(boost::to_lower_copy(iter->get_value(this->mTreeFilebrowser->mModelColumns.mColumnURI)), this->mNeedle));
}

FilebrowserFilter::~FilebrowserFilter() {
}