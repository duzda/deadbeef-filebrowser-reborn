#include "reader.hpp"

#include "plugin.hpp"

Glib::RefPtr<Gdk::Pixbuf> Reader::getImageFromData(TagLib::ByteVector bytes, int size) {
    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
    loader->set_size(size, size);
    try {
        loader->write((const unsigned char*)bytes.data(), bytes.size());
        loader->close();
        return loader->get_pixbuf();
    } catch (Glib::Error &e) {
        pluginLog(DDB_LOG_LAYER_INFO, ("Error while loading cover from metadata: " + e.what()));
    }

    // Return nullptr
    Glib::RefPtr<Gdk::Pixbuf> image;
    return image;
}

Reader::~Reader() {
}