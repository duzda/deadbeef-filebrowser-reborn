#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"

class Cache::Covers::MetadataFirst {
public:
    MetadataFirst();
    ~MetadataFirst();

    Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);
private:
};