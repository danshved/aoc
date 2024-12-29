#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool HasAt(const std::string& s, int i, const std::string& needle) {
    if (i + needle.length() > s.length()) {
        return false;
    }

    return std::equal(needle.begin(), needle.end(), s.begin() + i);
}

int main() {
    std::vector<std::string> input;
    std::ifstream in;
    std::string current;
    in.open("input.txt");
    while (std::getline(in, current)) {
        input.push_back(std::move(current));
    }
    in.close();

    int answer = 0;
    for (std::string s : input) {
        std::vector<int> nums;
        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '1' || HasAt(s, i, "one")) nums.push_back(1);
            if (s[i] == '2' || HasAt(s, i, "two")) nums.push_back(2);
            if (s[i] == '3' || HasAt(s, i, "three")) nums.push_back(3);
            if (s[i] == '4' || HasAt(s, i, "four")) nums.push_back(4);
            if (s[i] == '5' || HasAt(s, i, "five")) nums.push_back(5);
            if (s[i] == '6' || HasAt(s, i, "six")) nums.push_back(6);
            if (s[i] == '7' || HasAt(s, i, "seven")) nums.push_back(7);
            if (s[i] == '8' || HasAt(s, i, "eight")) nums.push_back(8);
            if (s[i] == '9' || HasAt(s, i, "nine")) nums.push_back(9);
            if (s[i] == '0') nums.push_back(0);
        }
        answer += 10 * nums.front() + nums.back();
    }

    std::cout << answer << std::endl;

    return 0;
}