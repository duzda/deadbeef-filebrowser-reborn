#pragma once

#include "id3v2reader.hpp"

class WavReader : public ID3v2Reader {
public:
    WavReader();
    ~WavReader();

    Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size);
private:
};