#include "FileUtils.h"
#include <fstream>

void FileUtils::write_to_file(const std::string &filename, const std::vector<std::string> &entries) {
    std::ofstream file(filename);
    for (const auto &entry : entries) {
        file << entry << std::endl;
    }
}
