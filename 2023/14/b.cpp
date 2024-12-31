#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "numbers.h"
#include "order.h"
#include "parse.h"

int GetLoad(const std::vector<std::string>& input) {
    int answer = 0;
    int height = input.size();
    int width = input[0].size();
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            if (input[i][j] == 'O') {
                answer += height - i;
            }
        }
    }
    return answer;
}

std::vector<std::string> Step(const std::vector<std::string>& input) {
    int height = input.size();
    int width = input[0].size();
    std::vector<std::string> output(width, std::string(height, '.'));
    for (int j = 0; j < width; j++) {
        std::vector<int> q;
        int q_index = 0;
        for (int i = 0; i < height; i++) {
            if (input[i][j] != '#') {
                q.push_back(i);
            } else {
                q.clear();
                q_index = 0;
                output[j][height - 1 - i] = '#';
            }
            if (input[i][j] == 'O') {
                int i1 = q[q_index++];
                output[j][height - 1 - i1] = 'O';
            }
        }
    }
    return output;
}

std::vector<std::string> Cycle(std::vector<std::string> v) {
    for (int i = 0; i < 4; i++) {
        v = Step(v);
    }
    return v;
}

std::string Cat(const std::vector<std::string>& ss) {
    std::ostringstream oss;
    for (const std::string& s : ss) {
        oss << s;
    }
    return oss.str();
}

int main() {
    std::vector<std::string> state = Split(Trim(GetContents("input.txt")), "\n");
    std::vector<std::vector<std::string>> states;

    std::unordered_map<std::string, int> index;
    int from = 0, to = 0;
    for (int i = 0; i < 1000000000; i++) {
        if (i % 1000000 == 0) {
            std::cout << "Cycle " << i << std::endl;
        }
        states.push_back(state);

        std::string s = Cat(state);
        auto it = index.find(s);
        if (it == index.end()) {
            index[s] = i;
        } else {
            from = it->second;
            to = i;
            break;
        }

        state = Cycle(std::move(state));
    }

    std::cout << GetLoad(states[from + (1000000000 - from) % (to - from)]) << std::endl;

    return 0;
}