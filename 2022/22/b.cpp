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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int Det(int a, int b, int c, int d) {
    return a * d - b * c;
}

// A point, vector, or 1x1x1 cell in 3d space.
//
// We assume a right-handed coordinate system: if the Z axis is pointing towards
// us then the shortest rotation from the X axis to the Y axis is counter-clockwise.
struct Vect {
    int x, y, z;

    Vect operator+(const Vect& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vect operator-() const {
        return {-x, -y, -z};
    }

    bool operator==(const Vect&) const = default;

    Vect Cross(const Vect& other) const {
        return {
            Det(y, z, other.y, other.z),
            Det(z, x, other.z, other.x),
            Det(x, y, other.x, other.y),
        };
    }

    bool InBounds(int size) const {
        return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
    }
};

template <>
struct std::hash<Vect> {
    size_t operator()(const Vect& v) const {
        return SeqHash(v.x, v.y, v.z);
    }
};

// Our position and orientation on the inner surface of the cube.
struct Frame {
    Vect pos;     // 1x1x1 cell just inside the cube. We're on one of its faces.
    Vect dir;     // Where we are looking.
    Vect normal;  // The inside normal (pointing from our feet to our head).

    Frame Step(int size) const {
        Vect next = pos + dir;
        if (next.InBounds(size)) {
            return {next, dir, normal};
        }
        return {pos, normal, -dir};
    }

    // Rotate clockwise as seen from inside the cube.
    Frame RotateRight() {
        return {pos, dir.Cross(normal), normal};
    }

    // Rotate counter-clockwise as seen from inside the cube.
    Frame RotateLeft() {
        return {pos, -dir.Cross(normal), normal};
    }
};

// An affine mapping from 3-d `Vect` to 2-d `Coord`.
struct Proj {
    Coord x;
    Coord y;
    Coord z;
    Coord origin;

    Coord MapVector(const Vect& vect) const {
        return x * vect.x + y * vect.y + z * vect.z;
    }

    Coord MapPoint(const Vect& vect) const {
        return MapVector(vect) + origin;
    }

    Coord MapCell(const Vect& vect) const {
        Coord a = MapPoint(vect), b = MapPoint(vect + Vect{1, 1, 1});
        return {std::min(a.i, b.i), std::min(a.j, b.j)};
    }
};

const std::unordered_map<Coord, int> kDirCodes = {
    {kEast, 0}, {kSouth, 1}, {kWest, 2}, {kNorth, 3}};

int main() {
    std::vector<std::string> lines = Split(GetContents("input.txt"), '\n');
    auto [matrix, bottom] = Split2(lines, std::string());
    assert(bottom.size() == 1);
    std::string commands = bottom[0];

    int size = (int)matrix[0].find('.');
    assert(size != std::string::npos);

    // Set up projections to unwrap the cube. Keys are inner normals. Values
    // are projections from our (x, y, z) space to the input (i, j) space,
    // where i is the row number and j is the column number. These projections
    // preserve orientation in the sense that looking at the puzzle input on
    // screen is "the same" as looking at the cube from the inside.

    // clang-format off
    std::unordered_map<Vect, Proj> projections = {
        {{0, 0,  1}, {.x = {0, 1}, .y = {-1,  0}, .z = { 0, 0}, .origin = {2 * size,     size}}},
        {{0, 0, -1}, {.x = {1, 0}, .y = { 0, -1}, .z = { 0, 0}, .origin = {3 * size,     size}}},
        {{0,  1, 0}, {.x = {0, 1}, .y = { 0,  0}, .z = { 1, 0}, .origin = {2 * size,     size}}},
        {{0, -1, 0}, {.x = {0, 1}, .y = { 0,  0}, .z = {-1, 0}, .origin = {    size,     size}}},
        {{ 1, 0, 0}, {.x = {0, 0}, .y = { 0, -1}, .z = { 1, 0}, .origin = {2 * size,     size}}},
        {{-1, 0, 0}, {.x = {0, 0}, .y = { 0, -1}, .z = {-1, 0}, .origin = {    size, 3 * size}}},
    };
    // clang-format on

    auto char_at = [&](const Frame& frame) {
        Coord c = projections.at(frame.normal).MapCell(frame.pos);
        return matrix[c.i][c.j];
    };
    auto step = [&](const Frame& u) {
        Frame v = u.Step(size);
        return char_at(v) == '#' ? u : v;
    };

    Frame state = {
        .pos = {0, size - 1, size - 1},
        .dir = {1, 0, 0},
        .normal = {0, -1, 0},
    };
    assert(char_at(state) == '.');

    auto end = commands.end();
    for (auto it = commands.begin(); it != end;) {
        if (*it == 'R') {
            state = state.RotateRight();
            it++;
        } else if (*it == 'L') {
            state = state.RotateLeft();
            it++;
        } else {
            auto next = std::find_if(it, end, [](char c) {
                return c == 'L' || c == 'R';
            });
            int steps = std::stoi(std::string(it, next));
            for (int i = 0; i < steps; i++) {
                state = step(state);
            }
            it = next;
        }
    }

    Coord pos = projections.at(state.normal).MapCell(state.pos);
    Coord dir = projections.at(state.normal).MapVector(state.dir);
    int answer = 1000 * (pos.i + 1) + 4 * (pos.j + 1) + kDirCodes.at(dir);
    std::cout << answer << std::endl;
    return 0;
}