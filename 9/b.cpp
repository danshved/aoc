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

#include "order.h"
#include "numbers.h"
#include "parse.h"

struct Span {
    int pos;
    int count;
};

int FindEmpty(const std::vector<int> disk, int len) {
    int i = 0;
    int count = 0;
    for (i = 0; i < disk.size(); i++) {
        if (disk[i] != -1) {
            count = 0;
            continue;
        }
        count++;
        if (count >= len) {
            return (i + 1 - count);
        }
    }
    return i;
}

int main() {
    std::string input = Trim(GetContents("input.txt"));
    int n = (input.length() + 1) / 2;


    std::vector<int> disk;
    std::vector<Span> files;

    int id = 0;
    for (int i = 0; i < input.length(); i++) {
        int cur = (i % 2 == 0) ? (id++) : -1;
        int len = input[i] - '0';

        if (cur != -1) {
            files.push_back(Span{(int)disk.size(), len});
        }

        for (int j = 0; j < len; j++) {
            disk.push_back(cur);
        }
    }
    assert (id == n);

    for (id = n - 1; id >= 0; id--) {
        int len = files[id].count;
        int pos = files[id].pos;

        int i = FindEmpty(disk, len);
        if (i < pos) {
            for (int j = 0; j < len; j++) {
                std::swap(disk[i + j], disk[pos + j]);
            }
        }
    }

    long long answer = 0;
    for (int i = 0; i < disk.size(); i++) {
        if (disk[i] == -1) {
            continue;
        }
        answer += i * disk[i];
    }
    std::cout << answer << std::endl;

    return 0;
}