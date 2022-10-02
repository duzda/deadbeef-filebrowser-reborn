#pragma once

#include <gtkmm.h>
#include <filesystem>

#include "reader.hpp"

/**
 * Abstracts all types metadata away, and provides simple getImage.
 */
class ReaderFactory {
public:
    /**
     * Returns image from metadata or generic audio icon, generic audio icon should not be cached.
     * 
     * @param path
     * @param size
     * @param shouldCache If there's image attached to metadata and we succesfully retrieved it, returned value is true, otherwise false.
     * 
     * @return Pointer to the image.
     */
    static Glib::RefPtr<Gdk::Pixbuf> getImage(std::filesystem::path path, int size, bool* shouldCache);
private:
    ReaderFactory();
    ~ReaderFactory();

    /**
     * Creates metadata reader for extension, otherwise returns nullptr.
     * 
     * @param extension Provide extensions with dot at the beggining, eg. .flac, .mp3.
     * 
     * @return Pointer to Reader object or nullptr.
     */
    static Reader* createReader(std::string extension);
};