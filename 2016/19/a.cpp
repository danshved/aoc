#include <iostream>
#include <vector>

int main() {
    int size = 3014387;
    std::vector<int> next(size, 0);
    for (int i = 0; i < size; i++) {
        next[i] = (i + 1) % size;
    }

    int cur = 0;
    while (next[cur] != cur) {
        cur = next[cur] = next[next[cur]];
    }
    std::cout << cur + 1 << std::endl;
    return 0;
}