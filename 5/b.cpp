#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <unordered_map>

std::vector<std::pair<int, int>> rules;
std::unordered_map<int, std::vector<int>> edges;

bool IsWellOrdered(const std::vector<int> update) {
    std::unordered_map<int, int> num_to_pos;
    for (int i = 0; i < update.size(); ++i) {
        num_to_pos[update[i]] = i + 1;
    }
    for (const std::pair<int, int>& rule : rules) {
        int pos_first = num_to_pos[rule.first];
        int pos_second = num_to_pos[rule.second];
        if (pos_first == 0 || pos_second == 0) {
            continue;
        }
        if (pos_first >= pos_second) {
            return false;
        }
    }
    return true;
}

std::unordered_map<int, bool> present;
std::unordered_map<int, bool> visited;
std::vector<int> ordered;

void Visit(int u) {
    visited[u] = true;
    for (int v : edges[u]) {
        if (present[v] && !visited[v]) {
            Visit(v);
        }
    }
    ordered.push_back(u);
}

std::vector<int> WellOrder(const std::vector<int> update) {
    visited.clear();
    ordered.clear();
    present.clear();
    for (int x : update) {
        present[x] = true;
    }
    for (int x : update) {
        if (!visited[x]) {
            Visit(x);
        }
    }
    std::reverse(ordered.begin(), ordered.end());
    return ordered;
}

int main() {

    std::vector<std::vector<int>> updates;

    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line), !line.empty()) {
        std::string a, b;
        std::istringstream line_s(line);
        std::getline(line_s, a, '|');
        std::getline(line_s, b, '|');
        int from = std::stoi(a), to = std::stoi(b);
        rules.push_back(std::make_pair(from, to));
        edges[from].push_back(to);
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
    for (const std::vector<int>& update : updates) {
        if (IsWellOrdered(update)) {
            continue;
        }

        std::vector<int> out = WellOrder(update);
        answer += out[out.size() / 2];
    }

    std::cout << answer << std::endl;
    return 0;
}