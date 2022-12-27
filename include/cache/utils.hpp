#pragma once

#include "cache.hpp"

#include <filesystem>

class Cache::Utils {
public:
    static std::filesystem::path createCacheBasePath();

    static void clearCache();
private:
    Utils();
    ~Utils();

    inline static const std::string APP_NAME = "filebrowser";
};