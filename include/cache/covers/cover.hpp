#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "cache.hpp"

/**
 * Inherit this class to implement your own algorithm to show cover arts and cache them.
 */
class Cache::Covers::Cover {
public:
    virtual ~Cover();

    /**
     * Promised to return cover art for the current path.
     * 
     * @return Pointer to the cover art.
     */
    virtual Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size) = 0;
private:
};