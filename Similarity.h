//
// Created by ??? on 24. 6. 16.
//

#ifndef UNTITLED_SIMILARITY_H
#define UNTITLED_SIMILARITY_H

#include "Processing_Tree.h"
#include <vector>
#include <queue>

using namespace std;

struct directory
{
    float weight;
    string word;
};

class Similarity {
private:
    string File_Name;
    vector<node> Duplicates;
    priority_queue<node> Results;

    void CalculateSimilarity(node& Folder_info);
    int calculateLevenshteinDistance(const string& s1, const string& s2);
    double calculateStringSimilarity(const string& token1, const string& token2);
    double calculateTokenSimilarity(const string& token1, const string& token2);
    vector<string> splitStringByDelimiter(const string& str, char delimiter);

public:
    Similarity(vector<node> Duplicates, string File_Name)
    {
        this->File_Name = File_Name;
        this->Duplicates = Duplicates;
    }
    void MakeOrder();
    priority_queue<node> getResult();
};


#endif //UNTITLED_SIMILARITY_H
