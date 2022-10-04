#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Extends Gtk::TreeView, most of the styling happends during initialization.
 */
class GUI::FBTreeView : public Gtk::TreeView {
public:
    FBTreeView();
    ~FBTreeView();

    void initialize(GUI::FBTreeModel* model, Glib::RefPtr<GUI::FBTreeFilter> filter);

    void setModel();

    /**
     * Returns selected URI from mTreeView.
     * 
     * @return URI of selected row
     */
    std::string getSelectedURI();

    /**
     * Returns all selected URIs from mTreeView.
     * 
     * @return Vector of selected rows
     */
    std::vector<std::string> getSelectedURIs();

    /**
     * Returns true if any row in mTreeView is selected.
     * 
     * @return True if any row is selected, false otherwise.
     */
    bool hasSelected();
private:
    using Gtk::TreeView::set_model;

    GUI::FBTreeModel* mModel;
    Glib::RefPtr<GUI::FBTreeFilter> mFilter;
};