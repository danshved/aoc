#include <iostream>

#include "parse.h"

int main() {
    int a = 48744869;
    std::vector<int> output;
    do {
        int low = a & 7;
        int high = (a >> (low ^ 2)) & 7;
        output.push_back(low ^ high ^ 1);
        a >>= 3;
    } while (a != 0);
    std::cout << FormatVector(output, ",") << std::endl;
}