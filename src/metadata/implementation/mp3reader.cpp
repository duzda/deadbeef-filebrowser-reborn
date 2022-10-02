#include "mp3reader.hpp"

#include <mpegfile.h>
#include <attachedpictureframe.h>

MP3Reader::MP3Reader() {
}

Glib::RefPtr<Gdk::Pixbuf> MP3Reader::getImage(std::filesystem::path path, int size) {
    TagLib::MPEG::File file(path.c_str());

    Glib::RefPtr<Gdk::Pixbuf> image;
    if (file.isValid()) {
        if (file.hasID3v2Tag()) {
            TagLib::ID3v2::Tag* tag = file.ID3v2Tag();
            image = this->getImageFromTag(tag, size);
            if (image.get()) {
                return image;
            }
        }
    }
    return image;
}

MP3Reader::~MP3Reader() {
}