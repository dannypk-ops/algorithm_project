
//
// Created by ���ر� on 24. 6. 16.
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

    // ��θ� '/' ������ �и��Ͽ� ���Ϳ� ����
    vector<string> parts = splitStringByDelimiter(Path, '\\');

    // �� ��� ���ο� _ ������ �ܾ���� �и��Ͽ�, ���� ����� ���� ����ġ ����
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


    // ������ ����ϰ�, Folder_info�� update.
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

// '/' �������� ���ڿ��� �и��Ͽ� ���Ϳ� �����ϴ� �Լ�
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
    // �� ��ū�� ���� ǥ���� ����ϴ�. ������ ���ڿ� ���̷� ��Ÿ���ڽ��ϴ�.
    vector<int> vec1(token1.size(), 1);
    vector<int> vec2(token2.size(), 1);

    // �� ������ ũ��(norm)�� ����մϴ�.
    double norm1 = 0.0, norm2 = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }

    // ������ ũ��(norm)�� 0�̸� NaN�� ��ȯ�մϴ�.
    if (norm1 == 0 || norm2 == 0) {
        return 0.0;  // �Ǵ� �ٸ� ������ ���� ��ȯ�� �� �ֽ��ϴ�.
    }

    // �� ������ ����(dot product)�� ����մϴ�.
    double dotProduct = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
    }

    // �� ������ ũ��(norm)�� ����մϴ�.
    norm1 = sqrt(norm1);
    norm2 = sqrt(norm2);

    // �ڻ��� ���絵�� ����մϴ�.
    double cosineSimilarity = dotProduct / (norm1 * norm2);

    return cosineSimilarity;
}

priority_queue<node> Similarity::getResult()
{
    return Results;
}
