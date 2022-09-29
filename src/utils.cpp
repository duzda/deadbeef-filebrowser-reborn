#include "utils.hpp"

#include "deadbeef/deadbeef.h"

#include <flacpicture.h>
#include <flacfile.h>
#include <id3v2tag.h>
#include <id3v1tag.h>
#include <attachedpictureframe.h>
#include <mpegfile.h>
#include <wavfile.h>
#include <oggfile.h>
#include <vorbisfile.h>

#include <boost/algorithm/string.hpp>

#include "filebrowser.hpp"
#include "plugin.hpp"
#include "settings.hpp"

Glib::RefPtr<Gdk::Pixbuf> Utils::getIcon(std::filesystem::path path, uint size) {
    Glib::RefPtr<Gdk::Pixbuf> icon;
    
    auto currentPath = Utils::createCachePath(path, size);

    if (std::filesystem::is_directory(path)) {
        std::string outputName;
        if (std::filesystem::exists(currentPath)) {
            return Gdk::Pixbuf::create_from_file(currentPath);
        } else if (Filebrowser::hasFile(path, {"cover.jpg", "cover.png", "front.jpg", "front.png"}, &outputName)) {
            icon = Gdk::Pixbuf::create_from_file(outputName, -1, size, true);
            icon->save(currentPath, "png");
            return icon;
        } else if (deadbeef->conf_get_int(FBR_LOADCOVER_ALBUM, 0) == 1) {
            auto fileVec = Filebrowser::getFileList(path, true, false);
            if (fileVec.size() > 0) {
                auto file = fileVec.front();
                if (file.is_regular_file() && file.file_size() > 0) {
                    try {
                        icon = Utils::getCoverPicture(file.path(), size);
                        icon->save(currentPath, "png");
                        return icon;
                    } catch (std::exception &e) {
                        return getIconByName("folder", size);
                    }
                }
            }
        }
        return Utils::getIconByName("folder", size);
    }

    if (std::filesystem::exists(currentPath)) {
        return Gdk::Pixbuf::create_from_file(currentPath);
    } else if (deadbeef->conf_get_int(FBR_LOADCOVER_ALBUM, 0) == 1) {
        try {
            icon = Utils::getCoverPicture(path, size);
            icon->save(currentPath, "png");
            return icon;
        } catch (std::exception &e) {
            return getIconByName("audio-x-generic", size);
        }
    }
    return Utils::getIconByName("audio-x-generic", size);
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
    //Also check XDG_CACHE_HOME as first default, home should be fallback or use the deadbeef cache
    //std::filesystem::path cache = std::filesystem::path(deadbeef->get_system_dir(DDB_SYS_DIR_CACHE));
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

Glib::RefPtr<Gdk::Pixbuf> Utils::getCoverPicture(std::string filename, int size) {
    std::string fileType = boost::to_lower_copy(filename.substr(filename.rfind(".")+1));
    
    if (fileType == "flac") {
        TagLib::FLAC::File f(filename.c_str());
        if (f.isValid()) {
            for(const auto &pic : f.pictureList()) {
                if (pic->height() > 0) {
                    unsigned int bufSize = pic->data().size();
                    guint8 *data = new guint8[bufSize];
                    memcpy(data, pic->data().data(), bufSize);
                        
                    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
                    loader->set_size(size, size);
                    try{
                        loader->write(data, bufSize);
                        loader->close();
                        delete[] data;
                        return loader->get_pixbuf();
                    } catch (Glib::Error &e) {
                        //pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: %s", e.what().c_str()));
                        std::cout << "Error while loading cover from metadata: " << e.what() << std::endl;
                        delete[] data;
                    }
                }
            }
            if (f.hasID3v2Tag()) {
                TagLib::ID3v2::Tag *tag = f.ID3v2Tag();
                TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
                for(const auto &pic : frameList) {
                    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pic);
                    if (frame->picture().size() > 0) {
                        unsigned int bufSize = frame->picture().size();
                        guint8 *data = new guint8[bufSize];
                        memcpy(data, frame->picture().data(), bufSize);
                        
                        Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
                        loader->set_size(size, size);
                        try{
                            loader->write(data, bufSize);
                            loader->close();
                            delete[] data;
                            return loader->get_pixbuf();
                        } catch (Glib::Error &e) {
                            //pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: %s", e.what().c_str()));
                            std::cout << "Error while loading cover from metadata: " << e.what() << std::endl;
                            delete[] data;
                        }
                    }
                }
            }
        }
    }
    if (fileType == "wav") {
        TagLib::RIFF::WAV::File f(filename.c_str());
        if (f.isValid()) {
            if (f.hasID3v2Tag()) {
                TagLib::ID3v2::Tag *tag = f.ID3v2Tag();
                TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
                for(const auto &pic : frameList) {
                    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pic);
                    if (frame->picture().size() > 0) {
                        unsigned int bufSize = frame->picture().size();
                        guint8 *data = new guint8[bufSize];
                        memcpy(data, frame->picture().data(), bufSize);
                        
                        Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
                        loader->set_size(size, size);
                        try{
                            loader->write(data, bufSize);
                            loader->close();
                            delete[] data;
                            return loader->get_pixbuf();
                        } catch (Glib::Error &e) {
                            //pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: %s", e.what().c_str()));
                            std::cout << "Error while loading cover from metadata: " << e.what() << std::endl;
                            delete[] data;
                        }
                    }
                }
            }
        }
    }
    if (fileType == "mp3") {
        TagLib::MPEG::File f(filename.c_str());
        if (f.isValid()) {
            if (f.hasID3v2Tag()) {
                TagLib::ID3v2::Tag *tag = f.ID3v2Tag();
                TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
                for(const auto &pic : frameList) {
                    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pic);
                    if (frame->picture().size() > 0) {
                        unsigned int bufSize = frame->picture().size();
                        guint8 *data = new guint8[bufSize];
                        memcpy(data, frame->picture().data(), bufSize);
                        
                        Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
                        loader->set_size(size, size);
                        try{
                            loader->write(data, bufSize);
                            loader->close();
                            delete[] data;
                            return loader->get_pixbuf();
                        } catch (Glib::Error &e) {
                            //pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: %s", e.what().c_str()));
                            std::cout << "Error while loading cover from metadata: " << e.what() << std::endl;
                            delete[] data;
                        }
                    }
                }
            }
        }
    }
    if (fileType == "ogg") {
        TagLib::Ogg::Vorbis::File f(filename.c_str());
        if (f.isValid()) {
            if (f.tag()) {
                TagLib::Ogg::XiphComment *tag = f.tag();
                TagLib::List<TagLib::FLAC::Picture*> picList = tag->pictureList();
                for(const auto &pic : picList) {
                    if (pic->height() > 0) {
                        unsigned int bufSize = pic->data().size();
                        guint8 *data = new guint8[bufSize];
                        memcpy(data, pic->data().data(), bufSize);
                        
                        Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
                        loader->set_size(size, size);
                        try{
                            loader->write(data, bufSize);
                            loader->close();
                            delete[] data;
                            return loader->get_pixbuf();
                        } catch (Glib::Error &e) {
                            //pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: %s", e.what().c_str()));
                            std::cout << "Error while loading cover from metadata: " << e.what() << std::endl;
                            delete[] data;
                        }
                    }
                }
            }
        }
    }
    throw std::runtime_error("No cover found");
}