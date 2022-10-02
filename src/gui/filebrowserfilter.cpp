#include "filebrowserfilter.hpp"

#include <boost/algorithm/string.hpp>

FilebrowserFilter::FilebrowserFilter(Glib::RefPtr<Gtk::TreeModel> model) :
Gtk::TreeModelFilter(model) {
}

Glib::RefPtr<FilebrowserFilter> FilebrowserFilter::create(Glib::RefPtr<FilebrowserModel> treefb) {
    return Glib::RefPtr<FilebrowserFilter>(new FilebrowserFilter(treefb));
}

void FilebrowserFilter::setNeedle(std::string newNeedle) {
    this->mNeedle = boost::to_lower_copy(newNeedle);
    this->mFilebrowserModel->setNeedleState(this->mNeedle.empty());
    this->refilter();
}

void FilebrowserFilter::setModel(FilebrowserModel* treefb) {
    this->mFilebrowserModel = treefb;
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

    return (this->mNeedle.empty() || boost::algorithm::contains(boost::to_lower_copy(iter->get_value(this->mFilebrowserModel->mModelColumns.mColumnURI)), this->mNeedle));
}

FilebrowserFilter::~FilebrowserFilter() {
}