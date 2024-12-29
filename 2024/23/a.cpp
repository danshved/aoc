#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "parse.h"
#include "collections.h"

int main() {
    std::unordered_map<std::string, std::unordered_set<std::string>> edges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, "-");
        edges[l].insert(r);
        edges[r].insert(l);
    }

    std::unordered_set<NTuple<3, std::string>, TupleHasher> triangles;
    for (const auto& [a, a_out] : edges) {
        if (a[0] != 't') {
            continue;
        }
        for (const std::string& b : a_out) {
            for (const std::string& c : a_out) {
                if (c <= b) {
                    continue;
                }
                if (!edges[b].contains(c)) {
                    continue;
                }

                std::vector<std::string> v = {a, b, c};
                std::sort(v.begin(), v.end());
                triangles.insert({v[0], v[1], v[2]});
            }
        }
    }

    std::cout << triangles.size() << std::endl;
    return 0;
}