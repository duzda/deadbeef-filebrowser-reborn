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

    /**
     * Tries to retreive icon from path, shows a warning on failure and uses instead default icon of specified size.
     * if sucesfully retreived icon from path saves it to the cache.
     * 
     * @param path path to the icon
     * @param cachePath
     * @param defaultIcon 
     * @param size 
     * @return icon from path or defaultIcon.
     */
    static Glib::RefPtr<Gdk::Pixbuf> getIcon(const std::string& path, const std::string& cachePath, const std::string& defaultIcon, uint size);
    static Glib::RefPtr<Gdk::Pixbuf> getIcon(const std::filesystem::path& path, const std::string& cachePath, const std::string& defaultIcon, uint size);

    /**
     * Tries to retreive icon from path, shows a warning on failure and explains to the user that their slow cache may
     * be corrupted and how to clean is. Then uses the default icon of specified size.
     * 
     * @param path path to the icon
     * @param defaultIcon 
     * @param size 
     * @return icon from path or defaultIcon.
     */
    static Glib::RefPtr<Gdk::Pixbuf> getIconFromCache(const std::string& path, const std::string& defaultIcon, uint size);
    static Glib::RefPtr<Gdk::Pixbuf> getIconFromCache(const std::filesystem::path& path, const std::string& defaultIcon, uint size);
private:
    Utils();
    ~Utils();
};