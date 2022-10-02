#include <treeview.hpp>

TreeView::TreeView() {
}

void TreeView::initialize(FilebrowserModel* fbmodel) {
    this->append_column("Icon", fbmodel->mModelColumns.mColumnIcon);
    this->append_column("Name", fbmodel->mModelColumns.mColumnName);

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
}

TreeView::~TreeView() {
}