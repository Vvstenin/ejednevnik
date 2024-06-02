#ifndef DIARYDATA_H
#define DIARYDATA_H

#include <string>
#include <vector>
#include <map>

class DiaryData {
public:
    static void add_entry(const std::string &date, const std::string &entry);
    static void delete_entry(const std::string &date);
    static std::vector<std::string> get_entries(const std::string &date);

private:
    static std::map<std::string, std::vector<std::string>> entries;
};

#endif // DIARYDATA_H
