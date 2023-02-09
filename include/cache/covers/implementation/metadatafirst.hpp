#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"
#include "cover.hpp"

class Cache::Covers::MetadataFirst : public Cache::Covers::Cover {
public:
    MetadataFirst();
    ~MetadataFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(const std::filesystem::path& path, uint size);
private:
    bool isNotChild(const std::filesystem::path& parent, const std::filesystem::path& child) const;
    Glib::RefPtr<Gdk::Pixbuf> hasChildrenIcon(const std::filesystem::path& cachePath, const std::filesystem::path& path, uint size);
};