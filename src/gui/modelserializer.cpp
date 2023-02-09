#include "fbtreemodel.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace GUI;

template<class Archive>
void FBTreeModel::save(Archive &ar, const unsigned int version) const {
    std::vector<Row> rows;
    for (auto iter = this->children().begin(); iter != this->children().end(); iter++) {
        this->saveRecursively(ar, &rows, iter, 0, version);
    }
    ar << rows;
}

template<class Archive>
void FBTreeModel::saveRecursively(Archive &ar, std::vector<Row>* rows, Gtk::TreeIter iter, int depth, const unsigned int version) const {
    Row row;
    row.depth = depth;
    auto icon = iter->get_value(this->ModelColumns.ColumnIcon);
    row.image.data.reserve(icon->get_byte_length());
    std::copy(icon->get_pixels(), icon->get_pixels() + icon->get_byte_length(), std::back_inserter(row.image.data));
    // All images are squares, we don't need to save height, just one dimension is enough
    row.image.size = icon->get_width();
    row.image.rowstride = icon->get_rowstride();
    row.image.has_alpha = icon->get_has_alpha();
    row.name = iter->get_value(this->ModelColumns.ColumnName);
    row.uri = iter->get_value(this->ModelColumns.ColumnURI);
    row.tooltip = iter->get_value(this->ModelColumns.ColumnTooltip);
    (*rows).push_back(row);

    if (!iter->children().empty()) {
        for (auto child = iter->children().begin(); child != iter->children().end(); child++) {
            this->saveRecursively(ar, rows, child, depth + 1, version);
        }
    }
}

template<class Archive>
void FBTreeModel::load(Archive &ar, const unsigned int version) {
    std::vector<Row> rows;
    Gtk::TreeModel::iterator iter;
    // Using ChildRow will free TreeRow from memory and make it unaccessible
    std::vector<Gtk::TreeRow> parentRows;
    int lastDepth = -1;
    ar >> rows;

    double progressTotal = rows.size();
    double progressIteration = 0.0;
    this->mThreadProgress = 0.0;

    for(const auto &row : rows) {
        if (!this->mRefreshThreadRunning.load()) {
            pluginLog(LogLevel::Info, "Load canceled by user");
            this->mThreadProgress = 1.0;
            this->mBridge->notify();
            this->mRefreshLock = false;
            return;
        }

        if (parentRows.size() > 0 && row.depth <= lastDepth) {
            // We always add latest TreeRow, we should pop it if the depth is equal, 
            // if it's lesser, we should pop all higher TreeRows
            for (int i = lastDepth; i > row.depth - 1; i--) {
                parentRows.pop_back();
            }
        }

        if (parentRows.size() > 0) {
            iter = this->append((parentRows[parentRows.size() - 1].children()));
        } else {
            iter = this->append();
        }

        Gtk::TreeRow treeRow = *iter;
        // We need to copy all the data on heap, as the original vector will get collected
        void* data = std::malloc(row.image.data.size());
        std::memcpy(data, row.image.data.data(), row.image.data.size());
        auto image = Gdk::Pixbuf::create_from_data(static_cast<const guint8*>(data), Gdk::COLORSPACE_RGB, row.image.has_alpha,
            8, row.image.size, row.image.size, row.image.rowstride);
        image->add_destroy_notify_callback(data, [](void* data) -> void* {
            std::free(data);
            return nullptr;
        });
        treeRow[this->ModelColumns.ColumnIcon] = image;
        treeRow[this->ModelColumns.ColumnName] = row.name;
        treeRow[this->ModelColumns.ColumnURI] = row.uri;
        treeRow[this->ModelColumns.ColumnTooltip] = row.tooltip;
        treeRow[this->ModelColumns.ColumnVisibility] = true;
        lastDepth = row.depth;
        
        parentRows.push_back(treeRow);

        this->mThreadProgress = progressIteration / progressTotal;
        this->mBridge->notify();
        progressIteration++;
    }

    pluginLog(LogLevel::Info, "Structure loaded");
    this->mRefreshLock = false;
    pluginLog(LogLevel::Info, "Notifying dispatcher - task done");
    this->mThreadProgress = 1.0;
    this->mBridge->notify();
}

template void FBTreeModel::save<boost::archive::binary_oarchive>(boost::archive::binary_oarchive &ar, const unsigned int) const;
template void FBTreeModel::saveRecursively<boost::archive::binary_oarchive>(boost::archive::binary_oarchive &ar, std::vector<Row>* rows, Gtk::TreeIter iter, int depth, const unsigned int version) const;
template void FBTreeModel::load<boost::archive::binary_iarchive>(boost::archive::binary_iarchive &ar, const unsigned int version);