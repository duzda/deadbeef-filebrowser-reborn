#pragma once

#define FBR_DEFAULT_PATH "filebrowser_reborn.default_path"
#define FBR_ICON_SIZE "filebrowser_reborn.icon_size"
#define FBR_DOUBLECLICK_REPLACE "filebrowser_reborn.doubleclick_replace"
#define FBR_ALBUM_ALGORITHM "filebrowser_reborn.album_algorithm"

#include <string>
#include <vector>

/**
 * Should be used for settings retrieval, for writing into settings, use deadbeef->conf_set_int etc.
 */
class Settings {
public:
    static Settings& getInstance();

    /**
     * Should be used as a callback to the changing of settings.
     */
    void onUpdate();

    std::string getDefaultPath();
    int getIconSize();
    int getDoubleclickReplace();
    int getAlbumAlgorithm();

    std::vector<std::string> getValidExtensions();
private:
    Settings();

    // Delete copy/move so extra instances can't be created/moved.
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;

    void update();

    std::string defaultPath;
    int iconSize;
    int doubleclickReplace;
    int albumAlgorithm;

    std::vector<std::string> validExtensions;
};