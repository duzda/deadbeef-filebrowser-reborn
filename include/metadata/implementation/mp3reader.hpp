#pragma once

#include "id3v2reader.hpp"

class MP3Reader : public ID3v2Reader {
public:
    MP3Reader();
    ~MP3Reader();

    Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size);
private:
};