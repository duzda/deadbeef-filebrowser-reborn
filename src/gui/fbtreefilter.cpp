#include "fbtreefilter.hpp"

#include <boost/algorithm/string.hpp>

#include "fbtreemodel.hpp"

using namespace GUI;

FBTreeFilter::FBTreeFilter(Glib::RefPtr<FBTreeModel> model) :
Gtk::TreeModelFilter(model) {
    this->mModel = model.get();
    this->set_visible_func(mem_fun(*this, &FBTreeFilter::filter_func));
}

Glib::RefPtr<FBTreeFilter> FBTreeFilter::create(Glib::RefPtr<FBTreeModel> model) {
    return Glib::RefPtr<FBTreeFilter>(new FBTreeFilter(model));
}

void FBTreeFilter::setNeedle(std::string needle) {
    this->mNeedle = boost::to_lower_copy(needle);
    this->mModel->setNeedleState(this->mNeedle.empty());
    this->refilter();
}

bool FBTreeFilter::filter_func(Gtk::TreeIter iter) {
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

    return (this->mNeedle.empty() || boost::algorithm::contains(boost::to_lower_copy(iter->get_value(this->mModel->ModelColumns.ColumnURI)), this->mNeedle));
}

FBTreeFilter::~FBTreeFilter() {
}