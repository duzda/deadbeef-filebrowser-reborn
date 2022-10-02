#pragma once

#include <gtkmm.h>
#include <filesystem>

#include <tbytevector.h>

class Reader {
public:
    virtual ~Reader();

    virtual Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size) = 0;
protected:
    Glib::RefPtr<Gdk::Pixbuf> getImageFromData(TagLib::ByteVector bytes, int size);
};