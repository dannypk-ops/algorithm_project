#pragma once
#ifndef UNTITLED_PROCESSING_TREE_H
#define UNTITLED_PROCESSING_TREE_H



#include <iostream>
#include <string>
#include <deque>
#include <vector>

extern std::string str_01;
extern std::string str_02;
extern std::string Only_Directory;
extern std::string start_directory;

typedef size_t KEY_TYPE;

struct node {
    std::string Folder_Name;
    std::string path;
    KEY_TYPE hash_value;
    double Similarity = 0;
    bool operator<(const node& other) const {
        // Similarity 기준으로 정렬
        return Similarity < other.Similarity;
    }
};
typedef struct node node;
typedef node* node_ptr;

extern std::vector<node> Folder_List;
extern std::deque<std::string> Hierarchy;

void ProcessingLine(std::string line);
void ExtractingLine(std::string line);
int calculateDepth(std::string line);

#endif // UNTITLED_PROCESSING_TREE_H
