#include "coverfirst.hpp"

#include "readerfactory.hpp"
#include "cache/covers/utils.hpp"
#include "iconutils.hpp"
#include "filebrowser.hpp"

using namespace Cache::Covers;

CoverFirst::CoverFirst() {
}

Glib::RefPtr<Gdk::Pixbuf> CoverFirst::getIcon(const std::filesystem::path& path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    static Glib::RefPtr<Gdk::Pixbuf> lastIcon;
    static std::string lastDirectory;

    auto currentPath = Utils::createCachePath(path, size);
    if (std::filesystem::exists(currentPath)) {
        icon = Cache::Covers::Utils::getIconFromCache(currentPath, "folder", size);

        lastIcon = icon;
        lastDirectory = path;
    } else {
        if (this->isNotChild(lastDirectory, path)) {
            if (std::filesystem::is_directory(path)) {
                std::string outputName;
                if (Filebrowser::hasFile(path, SUPPORTED_COVERS, &outputName)) {
                    icon = Cache::Covers::Utils::getIcon(outputName, currentPath, "folder", size);

                    lastDirectory = path;
                    lastIcon = icon;
                } else {
                    icon = IconUtils::getIconByName("folder", size);
                }
            } else {
                bool useCache = false;
                icon = ReaderFactory::getImage(path, size, &useCache);
                if (useCache) {
                    icon->save(currentPath, "bmp");
                }
            }
        } else {
            icon = lastIcon;
        }
    }
    return icon;
}

bool CoverFirst::isNotChild(const std::filesystem::path& parent, const std::filesystem::path& child) const {
    return parent == "" || child.string().find(parent) == std::string::npos || 
        child.string().size() <= parent.string().size() || child.string()[parent.string().size()] != '/';
}

CoverFirst::~CoverFirst() {
}
