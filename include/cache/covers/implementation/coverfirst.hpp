#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"
#include "cover.hpp"

class Cache::Covers::CoverFirst : Cache::Covers::Cover {
public:
    CoverFirst();
    ~CoverFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);
private:
    bool isNotChild(std::filesystem::path parent, std::filesystem::path child);
};