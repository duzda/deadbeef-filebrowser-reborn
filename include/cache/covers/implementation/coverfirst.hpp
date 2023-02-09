#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"
#include "cover.hpp"

class Cache::Covers::CoverFirst : public Cache::Covers::Cover {
public:
    CoverFirst();
    ~CoverFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(const std::filesystem::path& path, uint size);
private:
    bool isNotChild(const std::filesystem::path& parent, const std::filesystem::path& child) const;
};