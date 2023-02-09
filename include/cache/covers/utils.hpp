#pragma once

#include "cache.hpp"

#include <gtkmm.h>
#include <filesystem>

class Cache::Covers::Utils {
public:
    /**
     * Construct path to cache file for path at desired resolution.
     * 
     * The full path to the file is:
     * ~/.cache/deadbeef/filebrowser/icons/size/filename
     * 
     * @param path Path of the current folder we need to create cache file for.
     * @param size Resolution for the cache file.
     * 
     * @return Path to the cache file
     */
    static std::filesystem::path createCachePath(const std::filesystem::path& path, uint size);
private:
    Utils();
    ~Utils();
};