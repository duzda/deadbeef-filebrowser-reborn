#include "readerfactory.hpp"

#include "flacreader.hpp"
#include "mp3reader.hpp"
#include "oggreader.hpp"
#include "wavreader.hpp"

#include "utils.hpp"

#include <boost/algorithm/string.hpp>

Glib::RefPtr<Gdk::Pixbuf> ReaderFactory::getImage(std::filesystem::path path, int size, bool* shouldCache) {
    *shouldCache = false;
    if (!path.has_extension()) {
        return Utils::getIconByName("audio-x-generic", size);
    }

    std::string extension = path.extension();
    boost::to_lower(extension);

    Reader* reader = ReaderFactory::createReader(extension);

    if (reader) {
        Glib::RefPtr<Gdk::Pixbuf> image;
        image = reader->getImage(path, size);
        delete reader;

        if (image.get()) {
            *shouldCache = true;
            return image;
        }
    }

    return Utils::getIconByName("audio-x-generic", size);
}

Reader* ReaderFactory::createReader(std::string extension) {
    Reader* reader = nullptr;
    if (extension == ".flac") {
        reader = new FlacReader(); 
    } else if (extension == ".mp3") {
        reader = new MP3Reader();
    } else if (extension == ".ogg") {
        reader = new OggReader();
    } else if (extension == ".wav") {
        reader = new WavReader();
    }

    return reader;
}