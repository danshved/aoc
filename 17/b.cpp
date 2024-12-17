#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

using ULL = unsigned long long;

std::vector<ULL> program = {2,4,1,2,7,5,1,3,4,4,5,5,0,3,3,0};
ULL answer = std::numeric_limits<ULL>::max();

void solve(ULL a, ULL fixed, int shift, int pc) {
    if (pc == program.size()) {
        answer = std::min(answer, a);
        return;
    }
    for (ULL low = 0; low < 8; low++) {
        ULL a2 = (a & ~(0x7ull << shift)) | (low << shift);
        if ((a2 ^ a) & fixed) {
            continue;
        }
        ULL new_fixed = fixed | (0x7ull << shift);

        ULL f = (a2 >> shift) & 0x7ull;
        ULL piece = program[pc] ^ f ^ 0x1ull;
        int piece_shift = shift + (f ^ 0x2ull);
        ULL a3 = (a2 & ~(0x7ull << piece_shift)) | (piece << piece_shift);
        if ((a3 ^ a2) & new_fixed) {
            continue;
        }
        new_fixed |= 0x7ull << piece_shift;
        
        solve(a3, new_fixed, shift + 3, pc + 1);
    }
}

int main() {
    solve(0, 0, 0, 0);
    std::cout << answer << std::endl;
    return 0;
}