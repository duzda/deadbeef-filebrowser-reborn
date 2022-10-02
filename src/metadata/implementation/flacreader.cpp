#include "flacreader.hpp"

#include <flacfile.h>
#include <attachedpictureframe.h>

FlacReader::FlacReader() {
}

Glib::RefPtr<Gdk::Pixbuf> FlacReader::getImage(std::filesystem::path path, int size) {
    TagLib::FLAC::File file(path.c_str());

    Glib::RefPtr<Gdk::Pixbuf> image;
    if (file.isValid()) {
        for (const auto &picture : file.pictureList()) {
            image = this->getImageFromData(picture->data(), size);
            if (image.get()) {
                return image;
            }
        }
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

FlacReader::~FlacReader() {
}