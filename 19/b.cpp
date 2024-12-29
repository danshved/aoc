#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "parse.h"

int main() {
    auto [top, queries] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});
    std::vector<std::string> pieces = Split(top[0], ",");
    for (std::string& s : pieces) {
        s = Trim(s);
    }

    long long answer = 0;
    for (const std::string& s : queries) {
        std::vector<long long> d(s.size() + 1, 0);
        d[s.size()] = 1;
        for (int i = s.size() - 1; i >= 0; i--) {
            for (const std::string& piece : pieces) {
                if (i + piece.size() > s.size()) {
                    continue;
                }
                if (std::equal(piece.begin(), piece.end(), s.begin() + i)) {
                    d[i] += d[i + piece.size()];
                }
            }
        }
        answer += d[0];
    }

    std::cout << answer << std::endl;
    return 0;
}