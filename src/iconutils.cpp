#include "iconutils.hpp"

#include "settings.hpp"
#include "coverfactory.hpp"
#include "cover.hpp"

Glib::RefPtr<Gdk::Pixbuf> IconUtils::getIconByName(const char* name, uint size) {
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

Glib::RefPtr<Gdk::Pixbuf> IconUtils::getIcon(std::filesystem::path path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;

    int algorithm = Settings::getInstance().getAlbumAlgorithm();
    auto coverRetriever = Cache::Covers::CoverFactory::getCoverAlgorithm(algorithm);
    icon = coverRetriever->getIcon(path, size);

    return icon;
}