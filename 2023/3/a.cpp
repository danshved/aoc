#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

bool IsDigit(char c) {
    return std::isdigit(c);
}

bool IsSymbol(char c) {
    return (c != '.') && !IsDigit(c);
}

int main() {
    std::vector<std::string> input;
    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line)) {
        input.push_back(std::move(line));
    }
    in.close();

    int height = input.size();
    int width = input.front().size();
    std::map<std::pair<int, int>, std::vector<int>> gear_to_numbers;
    for (int i = 0; i < height; ++i) {
        int j1 = 0;
        while (j1 < width) {
            if (!std::isdigit(input[i][j1])) {
                j1++;
                continue;
            }
            int j2 = std::find_if_not(input[i].begin() + j1, input[i].end(), IsDigit) - input[i].begin();
            int part_num = std::stoi(std::string(input[i].begin() + j1, input[i].begin() + j2));

            for (int neigh_i = std::max(0, i - 1); neigh_i <= std::min(i + 1, height - 1); neigh_i++) {
                for (int neigh_j = std::max(0, j1 - 1); neigh_j <= std::min(j2, width - 1); neigh_j++) {
                    if (neigh_i == i && (neigh_j >= j1 && neigh_j < j2)) {
                        continue;
                    }
                    if (input[neigh_i][neigh_j] == '*') {
                        gear_to_numbers[std::make_pair(neigh_i, neigh_j)].push_back(part_num);
                    }
                }
            }

            j1 = j2;
        }
    }

    int answer = 0;
    for (const auto& it : gear_to_numbers) {
        const std::vector<int>& numbers = it.second;
        if (numbers.size() == 2) {
            answer += numbers[0] * numbers[1];
        }
    }
    std::cout << answer << std::endl;
    return 0;
}