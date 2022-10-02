#include "id3v2reader.hpp"

#include <attachedpictureframe.h>

Glib::RefPtr<Gdk::Pixbuf> ID3v2Reader::getImageFromTag(TagLib::ID3v2::Tag* tag, int size) {
    Glib::RefPtr<Gdk::Pixbuf> image;

    TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
    for(const auto &frame : frameList) {
        TagLib::ID3v2::AttachedPictureFrame* pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
        image = this->getImageFromData(pictureFrame->picture(), size);
        if (image.get()) {
            return image;
        }
    }
    return image;
}

ID3v2Reader::~ID3v2Reader() {
}