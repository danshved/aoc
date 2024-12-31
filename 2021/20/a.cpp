#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    auto [top, matrix] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});
    assert(top.size() == 1);
    std::string algo = std::move(top[0]);
    assert(algo.size() == 512);
    Box box = Sizes<2>(matrix);

    char outside = '.';
    std::unordered_map<Coord, char> image;
    for (Coord c : box) {
        image[c] = matrix[c.i][c.j];
    }
    auto get = [&](Coord c) -> char {
        return (image.contains(c)) ? image[c] : outside;
    };

    for (int step = 0; step < 2; step++) {
        std::unordered_map<Coord, char> new_image;
        box = Box({box.min_i - 1, box.min_j - 1, box.size_i + 2, box.size_j + 2});
        for (Coord u : box) {
            int code = 0;
            for (Coord v : Box{u.i - 1, u.j - 1, 3, 3}) {
                code = (get(v) == '#') ? (2 * code + 1) : (2 * code);
            }
            new_image[u] = algo[code];
        }
        image = std::move(new_image);
        outside = algo[(outside == '.') ? 0 : 511];
    }

    int answer = std::ranges::count_if(box, [&](Coord c) {
        return get(c) == '#';
    });
    std::cout << answer << std::endl;
    return 0;
}