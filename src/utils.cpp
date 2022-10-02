#include "utils.hpp"

#include "deadbeef/deadbeef.h"

#include "filebrowser.hpp"
#include "plugin.hpp"
#include "settings.hpp"
#include "coverfirst.hpp"

Glib::RefPtr<Gdk::Pixbuf> Utils::getIcon(std::filesystem::path path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;

    Cache::Covers::CoverFirst coverfirst;
    icon = coverfirst.getIcon(path, size);

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
    Glib::RefPtr<Gtk::IconTheme> theme = Gtk::IconTheme::get_default();
    Gtk::IconInfo icon = theme->lookup_icon(name, size);
    if (icon) {
        return icon.load_icon();
    } else {
        Glib::RefPtr<Gdk::Pixbuf> invalidIcon = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, size, size);
        invalidIcon->fill(0xFF00FFFF);
        return invalidIcon;
    }
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
    std::string cache = deadbeef->get_system_dir(DDB_SYS_DIR_CACHE);
    cache += "/filebrowser/icons/" + std::to_string(size) + std::string("/");

    if (!std::filesystem::exists(cache)) {
        std::filesystem::create_directories(cache);
    }

    cache += std::to_string(std::hash<std::string>{}(path.string()));

    return cache;
}