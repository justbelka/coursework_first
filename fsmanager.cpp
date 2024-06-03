#include "fsmanager.h"
#include <iostream>
#include <vector>
#include <filesystem>


namespace fs = std::filesystem;

std::vector<std::string> FSManager::ListDirectory(std::string path) {
    std::vector<std::string> files;

    if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            files.push_back(entry.path().filename().string());
        }
        if (size(files) == 0) {
            files.push_back("NOFILES");
        }
    }
    else {
        files.push_back("MISSINGDIRECTORY");
    }

    return files;
}

bool FSManager::IsExist(std::string path) {
    if (fs::exists(path)) {
        return 1;
    }
    else {
        return 0;
    }
}
