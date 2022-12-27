#include "cache/utils.hpp"

#include "plugin.hpp"

using namespace Cache;

std::filesystem::path Utils::createCacheBasePath() {
    std::string cache = deadbeef->get_system_dir(DDB_SYS_DIR_CACHE);
    cache += "/" + APP_NAME;
    return cache;
}

void Utils::clearCache() {
    auto cache = Utils::createCacheBasePath();

    if (std::filesystem::exists(cache)) {
        std::filesystem::remove_all(cache);
    }
}