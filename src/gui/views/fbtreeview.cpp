#include "fbtreeview.hpp"

#include "fbtreemodel.hpp"
#include "fbtreefilter.hpp"

using namespace GUI;

FBTreeView::FBTreeView() {
}

void FBTreeView::initialize(FBTreeModel* model, Glib::RefPtr<FBTreeFilter> filter) {
    this->append_column("Icon", model->ModelColumns.ColumnIcon);
    this->append_column("Name", model->ModelColumns.ColumnName);

    this->mModel = model;
    this->mFilter = filter;

    for (uint i = 0; i < 2; i++) {
        this->get_column(i)->set_spacing(0);
        this->get_column(i)->set_sizing(Gtk::TreeViewColumnSizing::TREE_VIEW_COLUMN_AUTOSIZE);
    }

    this->set_enable_search(true);
    this->set_search_column(1);
    this->set_expander_column(*this->get_column(0));
    this->set_show_expanders(true);
    this->set_enable_tree_lines(true);
    this->set_headers_visible(false);
    this->get_selection()->set_mode(Gtk::SelectionMode::SELECTION_MULTIPLE);
    this->set_has_tooltip(true);
    this->set_tooltip_column(3);

    this->setModel();
}

void FBTreeView::setModel() {
    this->set_model(this->mFilter);
}

std::string FBTreeView::getSelectedURI() {
    return mFilter->get_iter(this->get_selection()->get_selected_rows()[0])->get_value(mModel->ModelColumns.ColumnURI);
}

std::vector<std::string> FBTreeView::getSelectedURIs() {
    std::vector<std::string> uris;
    auto rows = this->get_selection()->get_selected_rows();
    for (auto &row : rows) {
        uris.push_back(mFilter->get_iter(row)->get_value(mModel->ModelColumns.ColumnURI));
    }

    return uris;
}

bool FBTreeView::hasSelected() {
    return this->get_selection()->get_selected_rows().size() != 0;
}

FBTreeView::~FBTreeView() {
}