#include "cache/covers/utils.hpp"

#include "cache/utils.hpp"
#include "plugin.hpp"
#include "iconutils.hpp"

using namespace Cache::Covers;

std::filesystem::path Utils::createCachePath(const std::filesystem::path& path, uint size) {
    std::string cache = Cache::Utils::createCacheBasePath().string() + "/icons/" + std::to_string(size) + "/";

    if (!std::filesystem::exists(cache)) {
        std::filesystem::create_directories(cache);
    }

    cache += std::to_string(std::hash<std::string>{}(path.string()));

    return cache;
}

Glib::RefPtr<Gdk::Pixbuf> Utils::getIcon(const std::string& path, const std::string& cachePath, const std::string& defaultIcon, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    try {
        icon = Gdk::Pixbuf::create_from_file(path, -1, size, true);
        icon->save(cachePath, "bmp");
    } catch (const Glib::Error& e) {
        pluginLog(LogLevel::Warning, "Error reading file: " + path);
        pluginLog(LogLevel::Warning, e.what());
        icon = IconUtils::getIconByName(defaultIcon.c_str(), size);
    } catch (...) {
        pluginLog(LogLevel::Warning, "Error reading file: " + path);
        icon = IconUtils::getIconByName(defaultIcon.c_str(), size);
    }
    return icon;
}

Glib::RefPtr<Gdk::Pixbuf> Utils::getIcon(const std::filesystem::path& path, const std::string& cachePath, const std::string& defaultIcon, uint size) {
    return Utils::getIcon(path.string(), cachePath, defaultIcon, size);
}

Glib::RefPtr<Gdk::Pixbuf> Utils::getIconFromCache(const std::string& path, const std::string& defaultIcon, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    try {
        icon = Gdk::Pixbuf::create_from_file(path);
    } catch (const Glib::Error& e) {
        pluginLog(LogLevel::Warning, "Slow cache is probably corrupted! Try deleting ~/.cache/deadbeef/filebrowser");
        pluginLog(LogLevel::Warning, "Error reading file: " + path);
        pluginLog(LogLevel::Warning, e.what());
        icon = IconUtils::getIconByName(defaultIcon.c_str(), size);
    } catch (...) {
        pluginLog(LogLevel::Warning, "Slow cache is probably corrupted! Try deleting ~/.cache/deadbeef/filebrowser");
        pluginLog(LogLevel::Warning, "Error reading file: " + path);
        icon = IconUtils::getIconByName(defaultIcon.c_str(), size);
    }
    return icon;
}

Glib::RefPtr<Gdk::Pixbuf> Utils::getIconFromCache(const std::filesystem::path& path, const std::string& defaultIcon, uint size) {
    return Utils::getIconFromCache(path.string(), defaultIcon, size);
}