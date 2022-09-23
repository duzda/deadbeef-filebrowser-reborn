#pragma once

#include <gtkmm.h>

#include "filebrowserfilter.hpp"

class Searchbar : public Gtk::Entry {
public:
    void setTreeModelFilter(FilebrowserFilter* filter);
    Searchbar();
    ~Searchbar();
private:
    FilebrowserFilter* mFilebrowserFilter;
    sigc::connection mTimer;

    const uint timeoutValue = 100;

    void on_change();
    bool on_timeout();
};