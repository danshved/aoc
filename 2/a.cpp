#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool IsSafe(const std::vector<int>& x) {
    if (x[0] == x[1]) {
        return false;
    }
    int mul = (x[0] > x[1]) ? -1 : 1;
    for(int i = 0; i + 1 < x.size(); ++i) {
        int diff = mul * (x[i + 1] - x[i]);
        if (diff < 1 || diff > 3) {
            return false;
        }
    }
    return true;
}

int main() {
    std::vector<std::vector<int>> x;
    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::vector<int> xs;
        std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(),
            std::back_inserter(xs));
        x.push_back(std::move(xs));
    }
    in.close();



    std::cout << std::count_if(x.begin(), x.end(), IsSafe) << std::endl;
    return 0;
}