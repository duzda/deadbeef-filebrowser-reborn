#pragma once 

#include <gtkmm.h>
#include <filesystem>

class Utils {
public:
    /**
     * Retrieves icon for current path.
     * 
     * If path is file, then the returned icon is audio file.
     * 
     * Otherwiser we check whether the path contains any cover file, 
     * if it contains cover file we try to retreive cover of desired 
     * resolution from cache, if no cache file exists we simply load 
     * the file from the path and create a new cache file.
     * 
     * If path doesn't contain any cover file, we simply return folder icon.
     * 
     * @param path Path that we need the icon for
     * @param size Resolution of the icon
     * 
     * @return Pointer to the icon.
     */
    static Glib::RefPtr<Gdk::Pixbuf> getIcon(std::filesystem::path path, uint size);

    /**
     * Retrieves local theme's gtk icon by it's name in desired resolution.
     * 
     * @param name Gtk icon name, see icons in Glade or documentation
     * @param size Desired resolution
     * 
     * @return Pointer to the icon, if name is resolved, nullptr otherwise.
     */
    static Glib::RefPtr<Gdk::Pixbuf> getIconByName(const char* name, uint size);

    /**
     * Replaces & with &amp; as defined by GTK.
     * 
     * @param tooltip String to be cleaned.
     * 
     * @return Edited string.
     */
    static std::string escapeTooltip(std::string tooltip);

    /**
     * Retrieves all supported extensions via deadbeef api
     * 
     * @return Vector of all supported extensions
     */
    static std::vector<std::string> createValidExtensions();

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
    static std::filesystem::path createCachePath(std::filesystem::path path, uint size);

    static void clearCache();
private:
    Utils();
    ~Utils();

    inline static const std::string APP_NAME = "filebrowser";
};
