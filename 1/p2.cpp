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
    auto x = xs.begin();
    auto y = ys.begin();
    while (x != xs.end() && y != ys.end()) {
        if (*x < *y) {
            x++;
        } else if (*x > *y) {
            y++;
        } else {
            auto x1 = std::find_if(x, xs.end(), [&](int t) {return t != *x;});
            auto y1 = std::find_if(y, ys.end(), [&](int t) {return t != *y;});
            answer += *x * (x1 - x) * (y1 - y);
            x = x1;
            y = y1;
        }
    }

    std::cout << answer << std::endl;
}