#include "settings.hpp"

#include "plugin.hpp"
#include "utils.hpp"

Settings::Settings() {
    this->update();
}

Settings& Settings::getInstance() {
    static Settings* singleton = new Settings();
    return *singleton;
}

void Settings::onUpdate() {
    int oldAlbumAlgorithm = this->albumAlgorithm;

    this->update();

     if (oldAlbumAlgorithm != this->albumAlgorithm) {
        Utils::clearCache();
     }
}

void Settings::update() {
    deadbeef->conf_lock();
    this->defaultPath = deadbeef->conf_get_str_fast(FBR_DEFAULT_PATH, "");
    deadbeef->conf_unlock();

    this->iconSize = deadbeef->conf_get_int(FBR_ICON_SIZE, 32);
    this->doubleclickReplace = deadbeef->conf_get_int(FBR_DOUBLECLICK_REPLACE, 0);
    this->albumAlgorithm = deadbeef->conf_get_int(FBR_ALBUM_ALGORITHM, 0);
}

std::string Settings::getDefaultPath() {
    return this->defaultPath;
}

int Settings::getIconSize() {
    return this->iconSize;
}

int Settings::getDoubleclickReplace() {
    return this->doubleclickReplace;
}

int Settings::getAlbumAlgorithm() {
    return this->albumAlgorithm;
}