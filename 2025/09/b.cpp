#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<Coord> points;
    std::unordered_set<int> interesting_i, interesting_j;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b] = SplitN(line, ",");
        Coord p{std::stoi(a), std::stoi(b)};
        points.push_back(p);
        interesting_i.insert(p.i);
        interesting_i.insert(p.i + 1);
        interesting_i.insert(p.i - 1);
        interesting_j.insert(p.j);
        interesting_j.insert(p.j + 1);
        interesting_j.insert(p.j - 1);
    }

    auto point_ok = [&](const Coord& p) {
        if (!interesting_i.contains(p.i) || !interesting_j.contains(p.j)) {
            return true;
        }

        int count = 0;
        for (int i = 0; i < points.size(); i++) {
            Coord a = points[i] - p, b = points[(i + 1) % points.size()] - p;
            if (a.i == b.i) {
                if (a.i == 0 && Sign(a.j) * Sign(b.j) <= 0) {
                    return true;
                }
                continue;
            }
            
            assert (a.j == b.j);
            if (a.j < 0) {
                continue;
            }
            if (a.j == 0) {
                if (Sign(a.i) * Sign(b.i) <= 0) {
                    return true;
                }
                continue;
            }
            if (a.i > 0 && b.i <= 0) {
                count++;
            } else if (b.i > 0 && a.i <= 0) {
                count--;
            }
        }
        return count != 0;
    };

    auto rect_ok = [&](const Coord& a, const Coord& b) {
        Coord p{a.i, b.j};
        Coord q{b.i, a.j};
        return std::ranges::all_of(PathCC(a, p), point_ok) && std::ranges::all_of(PathCC(p, b), point_ok) &&
            std::ranges::all_of(PathCC(b, q), point_ok) && std::ranges::all_of(PathCC(q, a), point_ok);
    };

    long long answer = 0;
    int checked = 0;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            checked++;
            if ((checked % 100) == 0) {
                std::cout << "Checked " << checked << std::endl;
            }

            if (!rect_ok(points[i], points[j])) {
                continue;
            }
            Coord diff = points[i] - points[j];
            long long area = (1 + abs(diff.i)) * static_cast<long long>(1 + abs(diff.j));
            answer = std::max(answer, area);
        }
    }
    std::cout << answer << std::endl;

    return 0;
}