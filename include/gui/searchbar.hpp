#pragma once

#include <gtkmm.h>

#include "gui.hpp"

/**
 * Extends Gtk::SearchEntry, communicates with FilebrowserFilter to filter out results.
 */
class GUI::Searchbar : public Gtk::SearchEntry {
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