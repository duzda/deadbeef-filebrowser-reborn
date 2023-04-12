#include "metadatafirst.hpp"

#include "readerfactory.hpp"
#include "cache/covers/utils.hpp"
#include "iconutils.hpp"
#include "filebrowser.hpp"

using namespace Cache::Covers;

MetadataFirst::MetadataFirst() {   
}

Glib::RefPtr<Gdk::Pixbuf> MetadataFirst::getIcon(const std::filesystem::path& path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    static std::string lastDirectory;

    auto currentPath = Utils::createCachePath(path, size);
    if (std::filesystem::exists(currentPath)) {
        icon = Cache::Covers::Utils::getIconFromCache(currentPath, "folder", size);

        lastDirectory = path;
    } else {
        if (this->isNotChild(lastDirectory, path)) {
            if (std::filesystem::is_directory(path)) {
                std::string outputName;
                if (Filebrowser::hasFile(path, SUPPORTED_COVERS, &outputName)) {
                    icon = Cache::Covers::Utils::getIcon(outputName, currentPath, "folder", size);
                } else {
                    icon = this->hasChildrenIcon(currentPath, path, size);
                }
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
                std::string outputName;
                if (Filebrowser::hasFile(path, SUPPORTED_COVERS, &outputName)) {
                    icon = Cache::Covers::Utils::getIcon(outputName, currentPath, "folder", size);
                } else {
                    icon = this->hasChildrenIcon(currentPath, path, size);
                }
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

bool MetadataFirst::isNotChild(const std::filesystem::path& parent, const std::filesystem::path& child) const {
    return parent == "" || child.string().find(parent) == std::string::npos || 
        child.string().size() <= parent.string().size() || child.string()[parent.string().size()] != '/';
}

Glib::RefPtr<Gdk::Pixbuf> MetadataFirst::hasChildrenIcon(const std::filesystem::path& cachePath, const std::filesystem::path& path, uint size) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
        if (entry.is_directory()) {
            std::string outputName;
            if (Filebrowser::hasFile(entry, SUPPORTED_COVERS, &outputName)) {
                auto icon = Cache::Covers::Utils::getIcon(outputName, cachePath, "folder", size);
                return icon;
            }
        } else {
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

MetadataFirst::~MetadataFirst() {
}