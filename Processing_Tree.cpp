#include "Processing_Tree.h"

std::string str_01 = "+---";
std::string str_02 = "\\---";
std::string Only_Directory;
std::string start_directory;
std::vector<node> Folder_List;
std::deque<std::string> Hierarchy;

void ProcessingLine(std::string line) {
    int pos = std::string::npos;
    if ((pos = line.find("+---")) != std::string::npos || (pos = line.find("\\---")) != std::string::npos ) {
        Only_Directory += line + "\n";
        ExtractingLine(line);
    } else if (((line.find("|") == std::string::npos) && (line.find(" ") == std::string::npos))) {
        Only_Directory += line + "\n";
        ExtractingLine(line);
    } else {
        return;
    }
}

int calculateDepth(std::string line) {
    int totalDepth = 0;
    int flag = 0;
    size_t pos = 0;
    size_t prev_pos = 0;

    while ((pos = line.find('|', pos)) != std::string::npos) {
        int spaceLength = pos - prev_pos - 1;

        if (flag == 0) {
            prev_pos = pos;
            pos++;
            totalDepth++;
            flag = 1;
            continue;
        }

        if ((spaceLength - 3) % 4 == 0) {
            int depth = (spaceLength - 3) / 4;
            totalDepth += (depth + 1);
        } else {
            std::cerr << "Invalid space length between '|' characters: " << spaceLength << std::endl;
            return -1;
        }

        prev_pos = pos;
        pos++;
    }

    size_t lastPatternPos = line.find("+---", prev_pos);
    if (lastPatternPos == std::string::npos) {
        lastPatternPos = line.find("\\---", prev_pos);
    }

    if (lastPatternPos == 0) {
        totalDepth = 1;
    } else if (lastPatternPos != std::string::npos) {
        int spaceLength = lastPatternPos - prev_pos - 1;

        if ((spaceLength - 3) % 4 == 0) {
            int depth = (spaceLength - 3) / 4;
            totalDepth += (depth + 1);
        } else {
            return 0;
        }
    }

    return totalDepth;
}

void ExtractingLine(std::string line) {
    int depth = calculateDepth(line);

    // size_t pos = line.find_last_of("-") + 1;
    size_t pos = line.find_last_of("+---") + 1;
    if (pos == std::string::npos)
        pos = line.find_last_of("\\---") + 1;

    std::string folderName = line.substr(pos);

    if (Hierarchy.size() > depth) {
        while (Hierarchy.size() > depth)
            Hierarchy.pop_back();
    }

    std::string fullPath = "";
    for (auto folder : Hierarchy) {
        if (folder != "C:.")
            fullPath.append("\\" + folder);
        else
            fullPath.append(start_directory);
    }

    if (folderName == "C:.")
        fullPath.append(start_directory);
    else
        fullPath.append("\\" + folderName);

    Hierarchy.push_back(folderName);

    node folderNode;
    folderNode.Folder_Name = folderName;
    folderNode.path = fullPath;
    Folder_List.push_back(folderNode);
}
