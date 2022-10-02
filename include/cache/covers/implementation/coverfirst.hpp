#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"

class Cache::Covers::CoverFirst {
public:
    CoverFirst();
    ~CoverFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);
private:
    bool isNotChild(std::filesystem::path parent, std::filesystem::path child);
};