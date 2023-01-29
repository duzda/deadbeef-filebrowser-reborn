#pragma once

#include "cache.hpp"

#include "fbtreemodel.hpp"

#include <filesystem>

class Cache::TreeModel::Serializer {
public:
    static void save(GUI::FBTreeModel* model);
    static void load(GUI::FBTreeModel* model);

    /**
     * Compares timestamp of cached tree model and path
     * @param path
     * 
     * @return True if tree should be recovered
    */
    static bool shouldRecover(std::filesystem::path path);
private:
    static bool exists();
    static std::filesystem::path getPath();
};