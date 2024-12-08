#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <unordered_map>

bool IsWellOrdered(const std::vector<int> update) {
    std::unordered_map<int, int> num_to_pos;
    for (int i = 0; i < update.size(); ++i) {
        std::cerr << update[i] << "->" << i << " ";
        num_to_pos[update[i]] = i + 1;
    }
    std::cerr << std::endl;
    for (const std::pair<int, int>& rule : rules) {
        int pos_first = num_to_pos[rule.first];
        int pos_second = num_to_pos[rule.second];
        if (pos_first == 0 || pos_second == 0) {
            continue;
        }
        if (pos_first >= pos_second) {
            std::cerr << rule.first << '|' << rule.second << ": " << num_to_pos[rule.first] << ">=" << num_to_pos[rule.second] << std::endl;
            return false;
        }
    }
    return true;
}

std::vector<std::pair<int, int>> rules;
std::vector<std::vector<int>> updates;

int main() {
    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line), !line.empty()) {
        std::string a, b;
        std::istringstream line_s(line);
        std::getline(line_s, a, '|');
        std::getline(line_s, b, '|');
        rules.push_back(std::make_pair(std::stoi(a), std::stoi(b)));
    }
    while (std::getline(in, line)) {
        std::istringstream line_s(line);
        std::vector<int> update;
        std::string cur;
        while (std::getline(line_s, cur, ',')) {
            update.push_back(std::stoi(cur));
        }
        updates.push_back(std::move(update));
    }
    in.close();

    int answer = 0;
    for(const std::vector<int>& update : updates) {
        if(IsWellOrdered(update, rules)) {
            answer += update[update.size() / 2];
        }
    }

    std::cout << answer << std::endl;
    return 0;
}