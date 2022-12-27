#include "metadataonly.hpp"

#include "readerfactory.hpp"
#include "cache/covers/utils.hpp"
#include "iconutils.hpp"
#include "filebrowser.hpp"

using namespace Cache::Covers;

MetadataOnly::MetadataOnly() {   
}

Glib::RefPtr<Gdk::Pixbuf> MetadataOnly::getIcon(std::filesystem::path path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    static std::string lastDirectory;

    auto currentPath = Utils::createCachePath(path, size);
    if (std::filesystem::exists(currentPath)) {
        icon = Gdk::Pixbuf::create_from_file(currentPath);
        lastDirectory = path;
    } else {
        if (this->isNotChild(lastDirectory, path)) {
            if (std::filesystem::is_directory(path)) {
                icon = this->hasChildrenIcon(currentPath, path, size);
                lastDirectory = path;
            } else {
                bool useCache = false;
                icon = ReaderFactory::getImage(path, size, &useCache);
                if (useCache) {
                    icon->save(currentPath, "bmp");
                }
            }
        } else {
            if (std::filesystem::is_directory(path)) {
                icon = this->hasChildrenIcon(currentPath, path, size);
            } else {
                bool useCache = false;
                icon = ReaderFactory::getImage(path, size, &useCache);
                if (useCache) {
                    icon->save(currentPath, "bmp");
                }
            }
        }
    }
    return icon;
}

bool MetadataOnly::isNotChild(std::filesystem::path parent, std::filesystem::path child) {
    return parent == "" || child.string().find(parent) == std::string::npos || 
        child.string().size() <= parent.string().size() || child.string()[parent.string().size()] != '/';
}

Glib::RefPtr<Gdk::Pixbuf> MetadataOnly::hasChildrenIcon(std::filesystem::path cachePath, std::filesystem::path path, uint size) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_directory()) {
            bool useCache = false;
            auto icon = ReaderFactory::getImage(entry, size, &useCache);
            if (useCache) {
                icon->save(cachePath, "bmp");
                return icon;
            }
        }
    }

    return IconUtils::getIconByName("folder", size);
}

MetadataOnly::~MetadataOnly() {
}