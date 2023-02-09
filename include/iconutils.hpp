#pragma once 

#include <gtkmm.h>
#include <filesystem>

class IconUtils {
public:
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
    static Glib::RefPtr<Gdk::Pixbuf> getIcon(const std::filesystem::path& path, uint size);
private:
    IconUtils();
    ~IconUtils();
};
