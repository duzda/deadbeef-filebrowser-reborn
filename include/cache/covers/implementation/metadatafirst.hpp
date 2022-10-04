#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"
#include "cover.hpp"

class Cache::Covers::MetadataFirst : Cache::Covers::Cover {
public:
    MetadataFirst();
    ~MetadataFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);
private:
};