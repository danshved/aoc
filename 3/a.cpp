#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

int main() {
    std::ifstream in;
    in.open("input.txt");
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();
    std::string s = buffer.str();

    std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
    auto muls_begin = std::sregex_iterator(s.begin(), s.end(), mul_regex);
    auto muls_end = std::sregex_iterator();

    int answer = 0;
    for (auto it = muls_begin; it != muls_end; ++it) {
        int x = std::stoi((*it)[1]);
        int y = std::stoi((*it)[2]);
        answer += x * y;
    }

    std::cout << answer << std::endl;
    return 0;
}