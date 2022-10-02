#pragma once

#include "id3v2reader.hpp"

class FlacReader : public ID3v2Reader {
public:
    FlacReader();
    ~FlacReader();

    Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size) override;
private:
};