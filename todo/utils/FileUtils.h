#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>

class FileUtils {
public:
    static void write_to_file(const std::string &filename, const std::vector<std::string> &entries);
};

#endif // FILEUTILS_H
