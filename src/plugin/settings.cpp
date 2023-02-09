#include "settings.hpp"

#include "plugin.hpp"
#include "cache/utils.hpp"
#include "serializer.hpp"

#include <gtkmm.h>

Settings::Settings() {
    this->update();
    this->validExtensions = Settings::createValidExtensions();
}

Settings& Settings::getInstance() {
    static Settings* singleton = new Settings();
    return *singleton;
}

void Settings::onUpdate() {
    int oldAlbumAlgorithm = this->albumAlgorithm;
    int oldIconSize = this->iconSize;

    this->update();

     if (oldAlbumAlgorithm != this->albumAlgorithm) {
        Cache::Utils::clearCache();
     }

     if (oldAlbumAlgorithm != this->albumAlgorithm || oldIconSize != this->iconSize) {
        Cache::TreeModel::Serializer::clearModel();
     }
}

void Settings::update() {
    deadbeef->conf_lock();
    this->defaultPath = deadbeef->conf_get_str_fast(FBR_DEFAULT_PATH, Glib::get_user_special_dir(Glib::UserDirectory::USER_DIRECTORY_MUSIC).c_str());
    deadbeef->conf_unlock();

    this->iconSize = deadbeef->conf_get_int(FBR_ICON_SIZE, 32);
    this->doubleclickReplace = deadbeef->conf_get_int(FBR_DOUBLECLICK_REPLACE, 0);
    this->albumAlgorithm = deadbeef->conf_get_int(FBR_ALBUM_ALGORITHM, 0);
}

std::string Settings::getDefaultPath() const {
    return this->defaultPath;
}

int Settings::getIconSize() const {
    return this->iconSize;
}

int Settings::getDoubleclickReplace() const {
    return this->doubleclickReplace;
}

int Settings::getAlbumAlgorithm() const {
    return this->albumAlgorithm;
}

std::vector<std::string> Settings::getValidExtensions() const {
    return this->validExtensions;
}

std::vector<std::string> Settings::createValidExtensions() {
    std::vector<std::string> extensions;

    // Array
    struct DB_decoder_s **decoders = deadbeef->plug_get_decoder_list();
    for (gint i = 0; decoders[i]; i++) {
        const char** exts = decoders[i]->exts;
        for (gint j = 0; exts[j]; j++) {
            // Does anybody use this ridicilously long extensions?
            GString *buf = g_string_sized_new(32);
            g_string_append_printf(buf, ".%s", exts[j]);
            extensions.push_back(buf->str);
        }
    }
    return extensions;
}