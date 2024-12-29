#ifndef __AOC_GRID_H__
#define __AOC_GRID_H__

#include <iterator>
#include <unordered_map>

#include "collections.h"

// Coordinates on a grid. i grows "down" or "towards the south".
// j grows "right" or "towards the east".
struct Coord {
    int i, j;

    Coord() : i(0), j(0) {}

    Coord(int the_i, int the_j) : i(the_i), j(the_j) {}

    Coord(const std::tuple<int, int>& t)
        : i(std::get<0>(t)), j(std::get<1>(t)) {};

    Coord(const Coord&) = default;
    Coord& operator=(const Coord&) = default;
    Coord(Coord&&) = default;
    Coord& operator=(Coord&&) = default;

    Coord& operator=(const std::tuple<int, int>& t) {
        i = std::get<0>(t);
        j = std::get<1>(t);
        return *this;
    }

    Coord operator+(const Coord& other) const {
        return {i + other.i, j + other.j};
    }

    Coord operator-(const Coord& other) const {
        return {i - other.i, j - other.j};
    }

    Coord operator*(int k) const {
        return {i * k, j * k};
    }

    friend Coord operator*(int k, const Coord& c) {
        return c * k;
    }

    Coord& operator+=(const Coord& other) {
        return *this = *this + other;
    }

    Coord& operator-=(const Coord& other) {
        return *this = *this - other;
    }

    Coord& operator*=(int k) {
        return *this = *this * k;
    }

    bool operator<=>(const Coord&) const = default;

    // Rotates 90 degrees "right", i.e. clockwise.
    Coord RotateRight() const {
        return {j, -i};
    }

    // Rotates 90 degrees "left", i.e. counterclockwise.
    Coord RotateLeft() const {
        return {-j, i};
    }

    // Rotates 180 degees.
    Coord Flip() const {
        return {-i, -j};
    }

    // Manhattan a.k.a. Taxicab metric.
    int Manhattan() const {
        return abs(i) + abs(j);
    }

    // Chessboard a.k.a. Chebyshev metric.
    int Chess() const {
        return std::max(abs(i), abs(j));
    }

    // Clamp to within the given chessboard metric.
    Coord ChessClamp(int max_chess) const {
        assert(max_chess >= 0);
        return {std::max(-max_chess, std::min(max_chess, i)),
                std::max(-max_chess, std::min(max_chess, j))};
    }
};

template <>
struct std::hash<Coord> {
    size_t operator()(const Coord& c) const {
        return SeqHash(c.i, c.j);
    }
};

const Coord kNorth = {-1, 0}, kSouth = {1, 0}, kWest = {0, -1}, kEast = {0, 1};
const Coord kDirs[4] = {kNorth, kWest, kSouth, kEast};
const std::unordered_map<char, Coord> kCharToDir =
    {{'^', kNorth}, {'v', kSouth}, {'<', kWest}, {'>', kEast}};

bool InBounds(const Coord& c, int size_i, int size_j) {
    return c.i >= 0 && c.i < size_i && c.j >= 0 && c.j < size_j;
}

// Helper object that presents the grid as a range:
//
// for (Coord c : Bounds(size_i, size_j)) {...}
class Bounds {
   public:
    class Iterator {
       public:
        using difference_type = std::ptrdiff_t;
        using value_type = Coord;

        Iterator() {}

        Iterator(const Coord& cur, int size_j) : cur_(cur), size_j_(size_j) {}

        Coord operator*() const {
            return cur_;
        }

        Iterator& operator++() {
            if (size_j_ > 0) {
                cur_.j++;
                while (cur_.j >= size_j_) {
                    cur_.j -= size_j_;
                    cur_.i++;
                }
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator current = *this;
            ++(*this);
            return current;
        }

        bool operator==(const Iterator& other) const {
            if (size_j_ != other.size_j_) {
                return false;
            }
            if (size_j_ <= 0) {
                return true;
            }
            return cur_ == other.cur_;
        }

       private:
        Coord cur_;
        int size_j_ = 0;
    };

    using iterator = Iterator;

    Bounds(int size_i, int size_j) : size_i_(size_i), size_j_(size_j) {
        assert(size_i >= 0);
        assert(size_j >= 0);
    }

    Iterator begin() const {
        return Iterator(Coord{0, 0}, size_j_);
    }

    Iterator end() const {
        return Iterator(Coord{size_i_, 0}, size_j_);
    }

   private:
    int size_i_;
    int size_j_;
};

// Advances the point along the path of non-decreasing Manhattan norm,
// starting at {0, 0} and covering the entire plane.
//
// Returns true if the point's Manhattan norm increased after this step.
bool ManhattanNext(Coord& c) {
    c += (c.i < 0 && c.j <= 0)   ? Coord{1, -1}
         : (c.i >= 0 && c.j < 0) ? Coord{1, 1}
         : (c.i > 0 && c.j >= 0) ? Coord{-1, 1}
         : (c.i <= 0 && c.j > 0) ? Coord{-1, -1}
                                 : Coord{0, 0};

    if (c.j == 0 && c.i >= 0) {
        c += {1, 0};
        return true;
    } else {
        return false;
    }
}

class ManhattanIterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = Coord;

    ManhattanIterator() {}

    ManhattanIterator(Coord start, Coord delta) : start_(start), delta_(delta) {}

    Coord operator*() const {
        return start_ + delta_;
    }

    ManhattanIterator& operator++() {
        ManhattanNext(delta_);
        return *this;
    }

    ManhattanIterator operator++(int) {
        ManhattanIterator current = *this;
        ++(*this);
        return current;
    }

    bool operator==(const ManhattanIterator&) const = default;

   private:
    Coord start_;
    Coord delta_;
};

// Infinite range going around a given cell in the order of non-decreasing
// Manhattan metric.
class ManhattanSpiral {
   public:
    using iterator = ManhattanIterator;

    ManhattanSpiral(const Coord& start) : start_(start) {}

    iterator begin() const {
        return iterator(start_, {0, 0});
    }

    std::unreachable_sentinel_t end() const {
        return std::unreachable_sentinel;
    }

   private:
    Coord start_;
};

// A range listing all points at the given Manhattan distance from the given
// point. This is a diamond shape iterated over counterclockwise.
class ManhattanCircle {
   public:
    using iterator = ManhattanIterator;

    ManhattanCircle(const Coord& start, int val) : start_(start), val_(val) {
        assert(val >= 0);
    }

    iterator begin() const {
        return iterator(start_, {val_, 0});
    }

    iterator end() const {
        return iterator(start_, {val_ + 1, 0});
    }

   private:
    Coord start_;
    int val_;
};

// Advances the point along the path of non-decreasing chessboard norm,
// starting at {0, 0} and covering the entire plane.
//
// Returns true if the point's Manhattan norm increased after this step.
bool ChessNext(Coord& c) {
    c += (c.j <= c.i && c.i < -c.j)     ? Coord{1, 0}
         : (-c.i <= c.j && c.j < c.i)   ? Coord{0, 1}
         : (-c.j <= -c.i && -c.i < c.j) ? Coord{-1, 0}
         : (c.i <= -c.j && -c.j < -c.i) ? Coord{0, -1}
                                        : Coord{0, 0};

    if (c.i == c.j && c.i >= 0) {
        c += {1, 1};
        return true;
    } else {
        return false;
    }
}

class ChessIterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = Coord;

    ChessIterator() {}

    ChessIterator(Coord start, Coord delta) : start_(start), delta_(delta) {}

    Coord operator*() const {
        return start_ + delta_;
    }

    ChessIterator& operator++() {
        ChessNext(delta_);
        return *this;
    }

    ChessIterator operator++(int) {
        ChessIterator current = *this;
        ++(*this);
        return current;
    }

    bool operator==(const ChessIterator&) const = default;

   private:
    Coord start_;
    Coord delta_;
};

// Infinite range going around a given cell in the order of non-decreasing
// chessboard metric.
class ChessSpiral {
   public:
    using iterator = ChessIterator;

    ChessSpiral(const Coord& start) : start_(start) {}

    iterator begin() const {
        return iterator(start_, {0, 0});
    }

    std::unreachable_sentinel_t end() const {
        return std::unreachable_sentinel;
    }

   private:
    Coord start_;
};

// A range listing all points at the given Chessboard distance from the given
// point. This is a square iterated over counterclockwise.
class ChessCircle {
   public:
    using iterator = ChessIterator;

    ChessCircle(const Coord& start, int val) : start_(start), val_(val) {
        assert(val >= 0);
    }

    iterator begin() const {
        return iterator(start_, {val_, val_});
    }

    iterator end() const {
        return iterator(start_, {val_ + 1, val_ + 1});
    }

   private:
    Coord start_;
    int val_;
};

// A range including 4 neighbors of this point (not diagonals),
// so that we can use for-loops like this:
//
// for (Coord v : Adj4(u)) {...}.
ManhattanCircle Adj4(const Coord& c) {
    return ManhattanCircle(c, 1);
}

// A range including 8 neighbors of this point (with diagonals),
// so that we can use for-loops like this:
//
// for (Coord v :Adj8(u)) {...}.
auto Adj8(const Coord& c) {
    return ChessCircle(c, 1);
}

// Shortest path between two Coords using straight and diagonal moves.
// The path is closed-open, i.e. the final point is not included.
class PathCO {
   public:
    class Iterator {
       public:
        using difference_type = std::ptrdiff_t;
        using value_type = Coord;

        Iterator() {}
        Iterator(const Coord& cur, const Coord& end) : cur_(cur), end_(end) {}

        Coord operator*() const {
            return cur_;
        }

        Iterator& operator++() {
            if (cur_ != end_) {
                cur_ += (end_ - cur_).ChessClamp(1);
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator current = *this;
            ++(*this);
            return current;
        }

        bool operator==(const Iterator&) const = default;

       private:
        Coord cur_;
        Coord end_;
    };
    using iterator = Iterator;

    Iterator begin() const {
        return Iterator(start_, end_);
    }

    Iterator end() const {
        return Iterator(end_, end_);
    }

    PathCO(const Coord& start, const Coord& end) : start_(start), end_(end) {}

   private:
    Coord start_;
    Coord end_;
};

// Shortest path between two Coords using straight and diagonal moves.
// The path is closed-closed, i.e. both the start and end points are included.
class PathCC {
   public:
    class Iterator {
       public:
        using difference_type = std::ptrdiff_t;
        using value_type = Coord;

        Iterator() {}
        Iterator(const std::optional<Coord>& cur, const Coord& end) : cur_(cur), end_(end) {}

        Coord operator*() const {
            return cur_.value();
        }

        Iterator& operator++() {
            if (!cur_.has_value()) {
                return *this;
            }
            if (*cur_ != end_) {
                *cur_ += (end_ - *cur_).ChessClamp(1);
            } else {
                cur_ = std::nullopt;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator current = *this;
            ++(*this);
            return current;
        }

        bool operator==(const Iterator&) const = default;

       private:
        std::optional<Coord> cur_;
        Coord end_;
    };
    using iterator = Iterator;

    Iterator begin() const {
        return Iterator(start_, end_);
    }

    auto end() const {
        return Iterator(std::nullopt, end_);
    }

    PathCC(const Coord& start, const Coord& end) : start_(start), end_(end) {}

   private:
    Coord start_;
    Coord end_;
};

// Convenient struct to represent a pair (position, direction).
struct PosDir {
    Coord pos;
    Coord dir;

    PosDir() {};

    PosDir(const Coord& the_pos, const Coord& the_dir)
        : pos(the_pos), dir(the_dir) {}

    bool operator<=>(const PosDir&) const = default;

    PosDir Step() const {
        return {pos + dir, dir};
    }

    PosDir StepBack() const {
        return {pos - dir, dir};
    }

    PosDir StrafeRight() const {
        return {pos + dir.RotateRight(), dir};
    }

    PosDir StrafeLeft() const {
        return {pos + dir.RotateLeft(), dir};
    }

    PosDir RotateRight() const {
        return {pos, dir.RotateRight()};
    }

    PosDir RotateLeft() const {
        return {pos, dir.RotateLeft()};
    }

    PosDir Flip() const {
        return {pos, dir.Flip()};
    }
};

template <>
struct std::hash<PosDir> {
    size_t operator()(const PosDir& s) const {
        return SeqHash(s.pos, s.dir);
    }
};

#endif