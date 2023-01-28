#include <boost/serialization/access.hpp>

#include "fbtreemodel.hpp"

BOOST_SERIALIZATION_SPLIT_FREE(GUI::FBTreeModel)

struct Image {
    std::vector<guint8> data;
    Gdk::Colorspace colorspace;
    gboolean has_alpha;
    int bits_per_sample;
    int width;
    int height;
    int rowstride;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & data & colorspace & has_alpha & bits_per_sample & width & height & rowstride;
    }
};

struct Row {
    int depth;
    std::string name;
    std::string uri;
    std::string tooltip;
    Image image;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & depth & name & uri & tooltip & image;
    }
};

namespace boost { namespace serialization {

template<class Archive>
void save(Archive &ar, const GUI::FBTreeModel &model, const unsigned int version) {
    std::vector<Row> rows;
    for (auto iter = model->children().begin(); iter != model->children().end(); iter++) {
        model->saveRecursively(ar, &rows, iter, 0, version);
    }
    ar << rows;
}

template<class Archive>
void saveRecursively(Archive &ar, const GUI::FBTreeModel &model, std::vector<Row>* rows, Gtk::TreeIter iter, int depth, const unsigned int version) {
    Row row;
    auto icon = iter->get_value(model->ModelColumns.ColumnIcon);
    row.depth = depth;
    row.image.data.reserve(icon->get_byte_length());
    pluginLog(LogLevel::Info, std::to_string(icon->get_byte_length()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_colorspace()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_has_alpha()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_bits_per_sample()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_width()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_height()));
    pluginLog(LogLevel::Info, std::to_string(icon->get_rowstride()));
    std::copy(icon->get_pixels(), icon->get_pixels() + icon->get_byte_length(), std::back_inserter(row.image.data));
    row.image.colorspace = icon->get_colorspace();
    row.image.has_alpha = icon->get_has_alpha();
    row.image.bits_per_sample = icon->get_bits_per_sample();
    row.image.width = icon->get_width();
    row.image.height = icon->get_height();
    row.image.rowstride = icon->get_rowstride();
    row.name = iter->get_value(model->ModelColumns.ColumnName);
    row.uri = iter->get_value(model->ModelColumns.ColumnURI);
    row.tooltip = iter->get_value(model->ModelColumns.ColumnTooltip);
    (*rows).push_back(row);

    if (!iter->children().empty()) {
        for (auto child = iter->children().begin(); child != iter->children().end(); child++) {
            saveRecursively(ar, rows, child, depth + 1, version);
        }
    }
}

template<class Archive>
void load(Archive &ar, const unsigned int version) {
    std::vector<Row> rows;
    Gtk::TreeModel::iterator iter;
    // Using ChildRow will free TreeRow from memory and make it unaccessible
    std::vector<Gtk::TreeRow> parentRows;
    int lastDepth = -1;
    ar >> rows;
    for(const auto &row : rows) {
        pluginLog(LogLevel::Info, std::to_string(row.image.data.size()));
        pluginLog(LogLevel::Info, std::to_string(row.image.colorspace));
        pluginLog(LogLevel::Info, std::to_string(row.image.has_alpha));
        pluginLog(LogLevel::Info, std::to_string(row.image.bits_per_sample));
        pluginLog(LogLevel::Info, std::to_string(row.image.width));
        pluginLog(LogLevel::Info, std::to_string(row.image.height));
        pluginLog(LogLevel::Info, std::to_string(row.image.rowstride));
        if (parentRows.size() > 0 && row.depth <= lastDepth) {
            // We always add latest TreeRow, we should pop it if the depth is equal, 
            // if it's lesser, we should pop all higher TreeRows
            for (int i = lastDepth; i > row.depth - 1; i--) {
                parentRows.pop_back();
            }
        }

        if (parentRows.size() > 0) {
            iter = model->append((parentRows[parentRows.size() - 1].children()));
        } else {
            iter = model->append();
        }

        Gtk::TreeRow treeRow = *iter;
        treeRow[model->ModelColumns.ColumnIcon] = Gdk::Pixbuf::create_from_data(row.image.data.data(), row.image.colorspace, row.image.has_alpha, 
            row.image.bits_per_sample, row.image.width, row.image.height, row.image.rowstride);
        treeRow[model->ModelColumns.ColumnName] = row.name;
        treeRow[model->ModelColumns.ColumnURI] = row.uri;
        treeRow[model->ModelColumns.ColumnTooltip] = row.tooltip;
        treeRow[model->ModelColumns.ColumnVisibility] = true;
        lastDepth = row.depth;
        
        parentRows.push_back(treeRow);
    }

    model->mView->setModel();
}

}} // namespace boost::serialization