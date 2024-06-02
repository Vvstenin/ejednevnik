#include "DiaryData.h"

std::map<std::string, std::vector<std::string>> DiaryData::entries;

void DiaryData::add_entry(const std::string &date, const std::string &entry) {
    entries[date].push_back(entry);
}

void DiaryData::delete_entry(const std::string &date) {
    if (entries.find(date) != entries.end()) {
        entries[date].pop_back();
        if (entries[date].empty()) {
            entries.erase(date);
        }
    }
}

std::vector<std::string> DiaryData::get_entries(const std::string &date) {
    if (entries.find(date) != entries.end()) {
        return entries[date];
    }
    return {};
}
