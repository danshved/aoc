#include <iostream>
#include <string>

#include "parse.h"

const long long kMod = 16777216;

long long Step(long long x) {
    x = (x ^ (x * 64)) % kMod;
    x = (x ^ (x / 32)) % kMod;
    x = (x ^ (x * 2048)) % kMod;
    return x;
}

int main() {
    long long answer = 0;
    for (const auto& line :  Split(Trim(GetContents("input.txt")), "\n")) {
        long long x = std::stoll(line);
        for (int i = 0; i < 2000; i++) {
            x = Step(x);
        }
        answer += x;
    }
    
    std::cout << answer << std::endl;
    return 0;
}