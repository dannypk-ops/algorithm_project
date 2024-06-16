#include "HashFunc.h"
#include <iostream>
#include <functional>

HashFunc::HashFunc() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

HashFunc::~HashFunc() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashEntry* entry = table[i];
        while (entry != nullptr) {
            HashEntry* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
}

int HashFunc::hashFunction(const std::string& key) {
    std::hash<std::string> hash_fn;
    size_t hashValue = hash_fn(key);
    return hashValue % TABLE_SIZE;
}

void HashFunc::insert(const std::string& folderName, const std::string& path) {
    int hashValue = hashFunction(folderName);
    //std::cout << "Inserting: " << folderName << " with path: " << path << " at hash value: " << hashValue << std::endl;

    HashEntry* newEntry = new HashEntry(folderName, path);
    if (!newEntry) {
        //std::cerr << "Memory allocation for new entry failed!" << std::endl;
        return;
    }

    if (table[hashValue] == nullptr) {
        table[hashValue] = newEntry;
        //std::cout << "Inserted at empty slot" << std::endl;
    }
    else {
        HashEntry* current = table[hashValue];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newEntry;
        //std::cout << "Inserted at linked list" << std::endl;
    }
}

std::string HashFunc::search(const std::string& folderName) {
    int hashValue = hashFunction(folderName);

    HashEntry* current = table[hashValue];
    while (current != nullptr) {
        if (current->key == folderName) {
            return current->path;
        }
        current = current->next;
    }
    return "";
}

void HashFunc::searchAllPaths(const std::string& folderName) {
    int hashValue = hashFunction(folderName);

    HashEntry* current = table[hashValue];
    while (current != nullptr) {
        if (current->key == folderName) {
            node new_node;
            new_node.Folder_Name = current->key;
            new_node.path = current->path;
            Result.push_back(new_node);
        }
        current = current->next;
    }
}

void HashFunc::displayAllPaths() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashEntry* current = table[i];
        while (current != nullptr) {
            std::cout << "Path: " << current->path << std::endl;
            current = current->next;
        }
    }
}

std::vector<node> HashFunc::getResult() {
    return Result;
}
