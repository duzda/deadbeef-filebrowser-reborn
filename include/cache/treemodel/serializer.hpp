#pragma once

#include "cache.hpp"

#include "fbtreemodel.hpp"

#include <filesystem>

class Cache::TreeModel::Serializer {
public:
    static void save(GUI::FBTreeModel* model);
    static void load(GUI::FBTreeModel* model);
    static bool exists();
private:
    static std::filesystem::path getPath();
};