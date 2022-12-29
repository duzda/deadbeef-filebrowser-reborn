#include "serializer.hpp"

#include "plugin.hpp"
#include "cache/utils.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace Cache::TreeModel;

void Serializer::save(GUI::FBTreeModel* model) {
    try {
        std::ofstream ofs(Serializer::getPath(), std::ios::binary);
        boost::archive::binary_oarchive oa(ofs);
        oa << (*model);
    } catch (const boost::archive::archive_exception &e) {
        pluginLog(LogLevel::Error, e.what());
    }
}

void Serializer::load(GUI::FBTreeModel* model) {
    try {
        std::ifstream ifs(Serializer::getPath(), std::ios::binary);
        boost::archive::binary_iarchive ia(ifs);
        ia >> (*model);
    } catch (const boost::archive::archive_exception &e) {
        pluginLog(LogLevel::Error, e.what());
    }
}

bool Serializer::exists() {
    return std::filesystem::exists(Serializer::getPath());
}

std::filesystem::path Serializer::getPath() {
    return Cache::Utils::createCacheBasePath().string() + "/treemodel";
}