#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"
#include "cover.hpp"

class Cache::Covers::MetadataOnly : public Cache::Covers::Cover {
public:
    MetadataOnly();
    ~MetadataOnly();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);
private:
    bool isNotChild(std::filesystem::path parent, std::filesystem::path child);
    Glib::RefPtr<Gdk::Pixbuf> hasChildrenIcon(std::filesystem::path cachePath, std::filesystem::path path, uint size);
};