#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "reader.hpp"

class ReaderFactory {
public:
    static Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size, bool* shouldCache);
private:
    ReaderFactory();
    ~ReaderFactory();

    static Reader* createReader(std::string extension);
};