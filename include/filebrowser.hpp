#pragma once

#include <filesystem>
#include <vector>
#include <string>

class Filebrowser {
public:
    /**
     * Retrieves files in directory. Beware as it retrieves only audio files specified by file extensions.
     * 
     * @param path Directory
     * @param sort Should the results be sorted? (Probably yes)
     * @param showHiddenFiles
     * 
     * @return Vector of directory entries for path
     */
    static std::vector<std::filesystem::directory_entry> getFileList(const std::filesystem::path& path, bool sort, bool showHiddenFiles);

    /**
     * @param directory
     * @param acceptedFiles
     * @param outputFile If return value is true, this pointer gets set to the file we found.
     * 
     * @return True if directory contains file, false otherwise.
     */
    static bool hasFile(const std::filesystem::path& directory, std::vector<std::string> acceptedFiles, std::string* outputFile);
private:
    Filebrowser();
    virtual ~Filebrowser();
};