#include <iostream>
#include <regex>

#include "parse.h"

int main() {
    std::string s = Trim(GetContents("input.txt"));
    std::regex mul_regex("(mul\\((\\d+),(\\d+)\\))|(do\\(\\))|(don't\\(\\))");
    auto muls_begin = std::sregex_iterator(s.begin(), s.end(), mul_regex);
    auto muls_end = std::sregex_iterator();

    int answer = 0;
    bool enabled = true;
    for (auto it = muls_begin; it != muls_end; ++it) {
        if (it->str() == "do()") {
            enabled = true;
        } else if (it->str() == "don't()") {
            enabled = false;
        } else if (enabled) {
            int x = std::stoi((*it)[2]);
            int y = std::stoi((*it)[3]);
            answer += x * y;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}