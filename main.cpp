

#include "Processing_Tree.h"
#include "Similarity.h"
#include "HashFunc.h"
#include <iostream>
#include <fstream>

#include <string>
#include <algorithm>
#include <deque>
#include <vector>
#include <sstream>
#include <direct.h>
#include <filesystem>
#include <cstddef>
#include <chrono>

#include "SplayTree.h"

namespace fs = std::filesystem;

//////////////////////////////////////////////////////////////////////////////////////////////////////

// <영준>

const char* rootPath = "C:\\Users\\User"; // tree.txt 파일을 만들고 싶은 위치(폴더 경로) 입력하세요
const char* src = "C:\\Users\\User\\Downloads"; // 다운로드 폴더 경로 입력하세요

vector<node> linearSearch(string Folder_Name); // 목표 폴더 리스트(이름 중복) 반환
node parent(node child); // 상위 폴더를 반환해주는
node select(vector<node> duplicate, string Folder_Name); // 중복된 폴더 중에서 선택
void moveFile(const char* fileName, const char* src, const char* dest);

//////////////////////////////////////////////////////////////////////////////////////////////////////


int main() {

    /* tree.txt 파일 생성해서 옮겨오기*/
    /*-------------------------------------------*/

    char originalDir[_MAX_PATH];

    // 현재 작업 디렉토리를 얻어옵니다.
    if (_getcwd(originalDir, _MAX_PATH) == nullptr) {
        std::cerr << "현재 작업 디렉토리를 얻어오는데 실패했습니다." << std::endl;
        return 1;
    }

    const char* orginal = originalDir;

    // file.txt 파일을 생성합니다.
    string command = "tree /a > file.txt"; /* 터미널 명령어 */
    if (_chdir(rootPath) == 0) {

        system(command.c_str());

    }

    // 원래 작업 디렉토리로 돌아갑니다.
    if (_chdir(originalDir) != 0) {
        std::cerr << "원래 작업 디렉토리로 돌아오는데 실패했습니다." << std::endl;
        return 1;
    }
    /*-------------------------------------------*/

    std::string path = "file.txt";
    std::string Current_Path = "current.txt";
    std::string line;


    // file.txt 파일 옮겨오기 & current.txt에 절대경로 쓰기
    /*-------------------------------------------*/

     moveFile(path.c_str(), rootPath, originalDir); // file.txt 파일 작업 디렉토리로 옮겨오기

    // 파일 출력 스트림 객체 생성 및 파일 열기
    ofstream currentTXT("current.txt");

    // 파일이 성공적으로 열렸는지 확인
    if (!currentTXT) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    // 파일에 문자열 쓰기
    currentTXT << rootPath;

    // 파일 닫기
    currentTXT.close();

    /*-------------------------------------------*/

    std::ifstream START(Current_Path);
    if (!START) {
        std::cout << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }
    getline(START, start_directory);
    START.close();

    std::cout << start_directory << std::endl;

    std::ifstream inputFile(path);
    if (!inputFile) {
        std::cout << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    int index = 0;
    while (getline(inputFile, line)) {
        index++;
        if (index == 1 || index == 2)
            continue;
        ProcessingLine(line);
    }
    inputFile.close();

        //for (auto item : Folder_List) {
        //    std::cout << "Name: " << item.Folder_Name << std::endl;
        //    std::cout << "Path: " << item.path << std::endl;
        //    std::cout << std::endl;
        //}

    std::cout << endl;
    std::cout << "이동시킬 파일 입력 : ";
    std::string fileName;
    std::cin >> fileName;
    const char* fileToMove = fileName.c_str();

    std::cout << "목표 폴더 입력 : ";
    string target;
    std::cin >> target;
    std::cout << std::endl;

    
    auto start = std::chrono::high_resolution_clock::now(); // 시작 시간 측정 


    /*-------------------------------------------------------------------*/
    // 01. HashMap
    /*HashFunc HashMap;
    for (const auto& item : Folder_List)
        HashMap.insert(item.Folder_Name, item.path);

    HashMap.searchAllPaths(target);
    vector<node> Result = HashMap.getResult();*/

    /*-------------------------------------------------------------------*/
    // 02. Linear Search
     //vector<node> Result(linearSearch(target));

    /*-------------------------------------------------------------------*/
    // 03. Binary Search Tree
    vector<node> Result;
    hash<string> hash_fn;
    SplayTree tree;
    splay* root = NULL;
    for (auto item : Folder_List)
    {
        root = tree.Insert(hash_fn(item.Folder_Name), root, item);
    }
    KEY_TYPE hashValue = hash_fn(target);
    root = tree.Search(hashValue, root);
    if (root && root->key == hashValue)
    {

        for (node n : root->items)
        {
            // cout << "folder name: " << n.Folder_Name << " Path: " << n.path << endl;
            Result.push_back(n);
        }
    }
    else
        cout << "File doesn't exist" << endl;
    /*-------------------------------------------------------------------*/


    // Similarity 구현
    string File_Name = "";
    Similarity sim(Result, File_Name);
    sim.MakeOrder();
    priority_queue<node> final_result = sim.getResult();
    vector<node> Real_Result;
    while (!final_result.empty()) {
        Real_Result.push_back(final_result.top());
        final_result.pop();
    }
    
    /*while (!final_result.empty())
    {
        node target_node = final_result.top();
        cout << "path: " << target_node.path << endl; 
        final_result.pop();
    }*/

    auto end = std::chrono::high_resolution_clock::now(); // 완료 시간 측정

    cout << "중복 폴더 개수 : " << Result.size() << endl;

    node dest = select(Real_Result, target); // 이름이 중복되는 폴더 중에 선택
    cout << "타겟 폴더의 경로 -> " << dest.path << endl; // 최종 폴더 경로 출력

    moveFile(fileToMove, src, dest.path.c_str()); // 파일 이동

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////

// 순차탐색
vector<node> linearSearch(string Folder_Name) {

    vector<node> duplicates; // 이름 중복 폴더 관리할 벡터

    for (const auto& folder : Folder_List) { // 타겟 폴더와 이름이 같은 폴더들을 찾기

        if (folder.Folder_Name.compare(Folder_Name) == 0)
            duplicates.push_back(folder);
    }

    // cout << "중복 폴더 개수 : " << duplicates.size() << endl;

    return duplicates;

}

// 상위 폴더 반환하는 함수
//node parent(node child) {
//
//    string childName = child.Folder_Name;
//    string childPath = child.path;
//
//    string parentName, parentPath;
//
//    // 자식 경로를 '\' 를 구분자로 분리
//    istringstream iss(childPath);
//    string token;
//    vector<string> tokens;
//    while (getline(iss, token, '\\')) {
//        tokens.push_back(token);
//    }
//
//    // cout << "자식 폴더 경로 토큰" << endl;
//    // for (const auto& t : tokens) {
//    //     cout << t << endl;
//    // }
//
//    if (tokens.size() >= 2) {
//        ostringstream oss;
//        for (size_t i = 0; i < tokens.size() - 1; ++i) {
//            oss << tokens[i] << "\\";
//        }
//        parentPath = oss.str();
//    }
//    else {
//        // 경로 문자열에 유효한 폴더가 없을 경우 빈 문자열 반환
//        parentPath = "";
//    }
//
//    // cout << "만들어진 경로 : " << parentPath << endl;
//
//    tokens.pop_back();
//    if (!tokens.empty()) {
//        parentName = tokens.back();
//    }
//    else {
//        // 경로 문자열에 유효한 폴더가 없을 경우 빈 문자열 반환
//        parentName = "";
//    }
//
//    node temp;
//    temp.Folder_Name = parentName;
//    temp.path = parentPath;
//
//    return temp;
//
//}

node select(vector<node> duplicates, string Folder_Name) {

    int i = 1;

    cout << "[상위 폴더 목록]" << endl;
    for (const auto& d : duplicates) {
        // node p = parent(d);
        node p = d;
        cout << i++ << ": " << p.Folder_Name << endl;
        cout << "-> path : " << p.path << endl << endl;
    }
    cout << endl;

    cout << "번호 입력 : ";
    int input;
    cin >> input;

    node temp;
    temp = duplicates[input - 1];

    return temp;

}

 // 파일을 이동시키는 함수
void moveFile(const char* fileName, const char* src, const char* dest) {

    // 원본 파일 경로
    fs::path source(string(src) + "\\" + string(fileName));
    // 대상 파일 경로
    fs::path destination(string(dest) + "\\" + string(fileName));

    try {
        // 파일 이동
        fs::rename(source, destination);
        std::cout << "파일을 성공적으로 옮겼습니다." << std::endl;
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "파일을 옮기는데 실패했습니다: " << e.what() << std::endl;
    }

}

