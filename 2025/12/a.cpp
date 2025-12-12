#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    NestedVector<2, std::string> input = Split(Split(Trim(GetContents("input.txt")), "\n"), {""});
    const int shape_count = input.size() - 1;

    int easy = 0;
    int hard = 0;
    int answer = 0;
    for (const std::string& case_str: input.back()) {
        auto [width_str, height_str, right] = SplitN(case_str, "x", ": ");
        int width = std::stoi(width_str);
        int height = std::stoi(height_str);
        std::vector<int> counts;
        int count = 0;
        for (const std::string& s : Split(right, " ")) {
            counts.push_back(std::stoi(s));
            count+= counts.back();
        }

        if (count <= (width / 3) * (height / 3)) {
            std::cout << count << ", " << width << " x " << height << std::endl;
            easy++;
            answer++;
            continue;
        }

        std::cout << count << " vs " << (width / 3) * (height / 3) << ", deficit: " << (count - (width / 3) * (height / 3)) << std::endl;
        hard++;
    }

    std::cout << "Easy: " << easy << ", hard: " << hard << std::endl;
    std::cout << "Answer: " << answer << std::endl;
    return 0;
}