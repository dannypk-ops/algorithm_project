
//
// Created by 박준규 on 24. 6. 16.
//

#include "Similarity.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void Similarity::MakeOrder()
{
    for (node& Folder_info : Duplicates)
    {
        CalculateSimilarity(Folder_info);
        Results.push(Folder_info);
    }
}

void Similarity::CalculateSimilarity(node& Folder_info)
{
    string Path = Folder_info.path;
    vector<directory> Folder_Split;
    double weight = 1.0;

    // 경로를 '/' 단위로 분리하여 벡터에 저장
    vector<string> parts = splitStringByDelimiter(Path, '\\');

    // 각 경로 내부에 _ 단위로 단어들을 분리하여, 점수 계산을 위한 가중치 설정
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) {
        vector<string> subparts = splitStringByDelimiter(*it, '_');
        for (const string& subpart : subparts) {
            directory dir;
            dir.weight = weight;
            dir.word = subpart;
            Folder_Split.push_back(dir);
        }
        weight = weight * 0.7;
    }


    // 점수를 계산하고, Folder_info에 update.
    vector<string> File_Split = splitStringByDelimiter(Path, '_');

    double point = 0;
    for (directory dir : Folder_Split)
    {
        for (string file : File_Split)
        {
            point += calculateTokenSimilarity(dir.word, file) * dir.weight;
        }
    }

    Folder_info.Similarity = point;
}

// '/' 기준으로 문자열을 분리하여 벡터에 저장하는 함수
vector<string> Similarity::splitStringByDelimiter(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);

    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
        //cout << token << endl;
    }

    //    for ( string str : tokens)
    //    {
    //        cout << "word : " << str << endl;
    //    }
    return tokens;
}

double Similarity::calculateTokenSimilarity(const string& token1, const string& token2) {
    // 각 토큰의 벡터 표현을 만듭니다. 간단히 문자열 길이로 나타내겠습니다.
    vector<int> vec1(token1.size(), 1);
    vector<int> vec2(token2.size(), 1);

    // 각 벡터의 크기(norm)을 계산합니다.
    double norm1 = 0.0, norm2 = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }

    // 벡터의 크기(norm)이 0이면 NaN을 반환합니다.
    if (norm1 == 0 || norm2 == 0) {
        return 0.0;  // 또는 다른 적절한 값을 반환할 수 있습니다.
    }

    // 각 벡터의 내적(dot product)을 계산합니다.
    double dotProduct = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
    }

    // 각 벡터의 크기(norm)을 계산합니다.
    norm1 = sqrt(norm1);
    norm2 = sqrt(norm2);

    // 코사인 유사도를 계산합니다.
    double cosineSimilarity = dotProduct / (norm1 * norm2);

    return cosineSimilarity;
}

priority_queue<node> Similarity::getResult()
{
    return Results;
}
