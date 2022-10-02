#pragma once

#include "reader.hpp"

#include <id3v2tag.h>

class ID3v2Reader : public Reader {
public:
    virtual ~ID3v2Reader();
protected:
    Glib::RefPtr<Gdk::Pixbuf> getImageFromTag(TagLib::ID3v2::Tag* tag, int size);
};