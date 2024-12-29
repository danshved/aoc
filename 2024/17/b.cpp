#include <iostream>
#include <limits>
#include <vector>

using ULL = unsigned long long;

std::vector<ULL> program = {2, 4, 1, 2, 7, 5, 1, 3, 4, 4, 5, 5, 0, 3, 3, 0};
ULL answer = std::numeric_limits<ULL>::max();

/*
The program does this:
do {
    int low = a & 7;
    int high = (a >> (low ^ 2)) & 7;
    output.push_back(low ^ high ^ 1);
    a >>= 3;
} while (a != 0);
*/

void solve(ULL a, ULL a_fixed, int pc) {
    if (pc == program.size()) {
        answer = std::min(answer, a);
        return;
    }
    for (ULL low = 0; low < 010; low++) {
        // Set 3 bits of A to `low` if it doesn't contradict the bits set earlier.
        int low_shift = 3 * pc;
        ULL a2 = (a & ~(07ull << low_shift)) | (low << low_shift);
        if ((a2 ^ a) & a_fixed) {
            continue;
        }
        ULL a2_fixed = a_fixed | (07ull << low_shift);

        // Set 3 bits of A to `high` if it doesn't contradict the bits set earlier.
        // There is only one value of `high` that will cause the program to
        // output the expected value.
        ULL high = program[pc] ^ low ^ 01ull;
        int high_shift = low_shift + (low ^ 02ull);
        ULL a3 = (a2 & ~(07ull << high_shift)) | (high << high_shift);
        if ((a3 ^ a2) & a2_fixed) {
            continue;
        }
        ULL a3_fixed = a2_fixed | (07ull << high_shift);

        solve(a3, a3_fixed, pc + 1);
    }
}

int main() {
    solve(0, ~((1ull << 3 * program.size()) - 1), 0);
    std::cout << answer << std::endl;
    return 0;
}