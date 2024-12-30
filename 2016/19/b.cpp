#include <iostream>
#include <vector>

int main() {
    int size = 3014387;
    std::vector<int> next(size, 0);
    for (int i = 0; i < size; i++) {
        next[i] = (i + 1) % size;
    }

    int cur = 0;
    int prev_opposite = size / 2 - 1;
    while (next[cur] != cur) {
        next[prev_opposite] = next[next[prev_opposite]];
        cur = next[cur];
        if (size % 2 == 1) {
            prev_opposite = next[prev_opposite];
        }
        size--;
    }
    std::cout << cur + 1 << std::endl;
    return 0;
}