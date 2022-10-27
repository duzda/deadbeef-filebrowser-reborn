#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Extends Gtk::Entry, communicates with FilebrowserFilter to filter out results.
 */
class GUI::Searchbar : public Gtk::Entry {
public:
    Searchbar();
    ~Searchbar();

    void initialize(GUI::FBTreeFilter* filter);
private:
    GUI::FBTreeFilter* mFilter;
    sigc::connection mTimer;

    const uint timeoutValue = 100;

    void on_change();
    bool on_timeout();
};