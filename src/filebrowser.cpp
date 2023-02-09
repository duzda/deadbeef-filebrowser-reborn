#include "filebrowser.hpp"

#include <string.h>
#include <algorithm>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

#include "settings.hpp"

std::vector<std::filesystem::directory_entry> Filebrowser::getFileList(const std::filesystem::path& path, bool sort, bool showHiddenFiles) {
    std::vector<std::filesystem::directory_entry> files = {};

    for(const auto &entry : std::filesystem::directory_iterator(path)) {
        struct stat buffer; 
        // Check permissions and errors
        if (stat(entry.path().c_str(), &buffer) != 0 || access(entry.path().c_str(), R_OK)) {
            continue;
        }

        // Hidden files
        if (!showHiddenFiles && entry.path().filename().string()[0] == '.') {
            continue;
        }

        // Add every directory, we can remove them later
        if (entry.is_directory()) {
            files.push_back(entry);
            continue;
        }

        // Check file extension
        bool validFile = false;
        for (auto &extension : Settings::getInstance().getValidExtensions()) {
            if (entry.path().extension() == extension) {
                validFile = true;
                break;
            }
        }
        if (validFile) {
            files.push_back(entry);
        }
    }

    if (sort) {
        std::sort(files.begin(), files.end(), 
            [](const std::filesystem::directory_entry& s1, const std::filesystem::directory_entry& s2) -> bool {
                return strcasecmp(s1.path().c_str(), s2.path().c_str()) < 0 ? true : false;
            });
    }

    return files;
}

bool Filebrowser::hasFile(const std::filesystem::path& directory, std::vector<std::string> acceptedFiles, std::string* outputFile) {
    auto path = directory.string();
    for (auto &file : acceptedFiles) {
        if (std::filesystem::exists(path + '/' + file)) {
            *outputFile = std::string(directory.string() + std::string("/") + file);
            return true;
        }
    }
    return false;
}