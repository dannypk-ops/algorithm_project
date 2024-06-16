#pragma once

#include <string>
#include <vector>
#include "Processing_Tree.h"

const int TABLE_SIZE = 4000;

struct HashEntry {
    std::string key;
    std::string path;
    HashEntry* next;

    HashEntry(const std::string& k, const std::string& p) : key(k), path(p), next(nullptr) {}
};

class HashFunc {
private:
    HashEntry* table[TABLE_SIZE];
    std::vector<node> Result;

    int hashFunction(const std::string& key);

public:
    HashFunc();
    ~HashFunc();

    void insert(const std::string& folderName, const std::string& path);
    std::string search(const std::string& folderName);
    void searchAllPaths(const std::string& folderName);
    void displayAllPaths();
    std::vector<node> getResult();
};
