#include "cache/covers/utils.hpp"

#include "cache/utils.hpp"

using namespace Cache::Covers;

std::filesystem::path Utils::createCachePath(const std::filesystem::path& path, uint size) {
    std::string cache = Cache::Utils::createCacheBasePath().string() + "/icons/" + std::to_string(size) + "/";

    if (!std::filesystem::exists(cache)) {
        std::filesystem::create_directories(cache);
    }

    cache += std::to_string(std::hash<std::string>{}(path.string()));

    return cache;
}