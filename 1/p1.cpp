#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> xs, ys;

    std::ifstream in;
    in.open("input.txt");
    for (int i = 0; i < 1000; ++i) {
        int x, y;
        in >> x >> y;
        xs.push_back(x);
        ys.push_back(y);
    }
    in.close();

    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    int answer = 0;
    for (int i = 0; i < xs.size(); ++i) {
        answer += std::abs(xs[i] - ys[i]);
    }

    std::cout << answer << std::endl;
}