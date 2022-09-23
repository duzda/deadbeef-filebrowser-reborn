#include "utils.hpp"

#include "deadbeef/deadbeef.h"

#include "filebrowser.hpp"
#include "plugin.hpp"

Glib::RefPtr<Gdk::Pixbuf> Utils::getIcon(std::filesystem::path path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;

    if (std::filesystem::is_directory(path)) {
        std::string outputName;
        auto currentPath = Utils::createCachePath(path, size);
        if (std::filesystem::exists(currentPath)) {
            icon = Gdk::Pixbuf::create_from_file(currentPath);
        } else if (Filebrowser::hasFile(path, {"cover.jpg", "cover.png", "front.jpg", "front.png"}, &outputName)) {
            icon = Gdk::Pixbuf::create_from_file(outputName, -1, size, true);
            icon->save(currentPath, "png");
        } else {
            icon = Utils::getIconByName("folder", size);
        }
    } else {
        icon = Utils::getIconByName("audio-x-generic", size);
    }

    return icon;
}

std::string Utils::escapeTooltip(std::string tooltip) {
    uint i = 0;
    std::string n = tooltip;
    // Memory leak? See Valgrind
    for (char const &c : n) {
        i++;
        if (c == '&') {
            n.insert(i, "amp;");
        }
    }
    return n;
}

Glib::RefPtr<Gdk::Pixbuf> Utils::getIconByName(const char* name, uint size) {
    auto theme = Gtk::IconTheme::get_default();
    return theme->load_icon(name, size);
}

std::vector<std::string> Utils::createValidExtensions() {
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

std::filesystem::path Utils::createCachePath(std::filesystem::path path, uint size) {
    std::filesystem::path cache = getenv("HOME");
    cache.append(".cache");
    cache.append(CACHE_PATH + "/icons/" + std::to_string(size) + std::string("/"));

    if (!std::filesystem::exists(cache)) {
        std::filesystem::create_directories(cache);
    }

    std::string editedPath = path.string();
    for (auto &c : editedPath) {
        if (c == '/') {
            c = '_';
        }
    }

    if (editedPath.size() > 255) {
        editedPath.resize(255);
    }
    cache.append(editedPath);
    return cache;
}