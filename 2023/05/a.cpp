#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <iterator>
#include <cctype>
#include <limits>

// Parse vector of numbers separated by spaces and/or commas.
template<typename T>
std::vector<T> ParseVector(std::string s) {
    for (char& c : s) {
        if (c == ',') {
            c = ' ';
        }
    }

    std::istringstream iss(s);
    std::vector<T> result;
    std::copy(std::istream_iterator<T>(iss), std::istream_iterator<T>(),
            std::back_inserter(result));
    return result;
}

template<typename T>
std::string FormatVector(const std::vector<T>& v, std::string delim = " ") {
    std::ostringstream oss;
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            oss << delim;
        }
        oss << *it;
    }
    return oss.str();
}

struct Range {
    long long start;
    long long end;
};

struct Triple {
    long long shift;
    Range source;

    bool operator<(const Triple& other) {
        return source.start < other.source.start;
    }
};

std::vector<std::vector<Triple>> transforms;
std::vector<Range> input_ranges;

long long Transform(long long x, const std::vector<Triple>& ts) {
    for (const Triple& t : ts) {
        if (x >= t.source.start && x < t.source.end) {
            return x + t.shift;
        }
    }
    return x;
}

std::vector<Range> TransformSet(const std::vector<Range> ranges, std::vector<Triple> ts) {
    std::sort(ts.begin(), ts.end());

    std::vector<Range> result;
    for (Range range : ranges) {
        for (const Triple& t : ts) {            
            // Left of t
            long long e = std::min(t.source.start, range.end);
            if (e > range.start) {
                result.push_back(Range{range.start, e});
            }

            // Intersection with t
            long long b = std::max(range.start, t.source.start);
            e = std::min(range.end, t.source.end);
            if (e > b) {
                result.push_back(Range{b + t.shift, e + t.shift});
            }

            // What's left is right of t
            range.start = std::max(range.start, t.source.end);
            if (range.start >= range.end) {
                break;
            }
        }
        if (range.start < range.end) {
            result.push_back(range);
        }
    }
    return result;
}

int main() {
    std::cout << sizeof(long long) << std::endl;

    std::ifstream in;
    in.open("input.txt");
    std::string line, dummy;
    std::getline(in, line);
    std::istringstream line_s(line);
    line_s >> dummy;
    std::vector<long long> xs;
    std::copy(std::istream_iterator<long long>(line_s),
            std::istream_iterator<long long>(),
            std::back_inserter(xs));
    for (int i = 0; i < xs.size() / 2; ++i) {
        input_ranges.push_back(Range{xs[2 * i], xs[2 * i] + xs[2 * i + 1]});
    }
    std::getline(in, line);

    while (true) {
        if (!std::getline(in, line)) {
            break;
        }
        std::vector<Triple> transform;
        while(std::getline(in, line) && !line.empty()) {
            std::istringstream line_s(line);
            long long dest_start, src_start, count;
            line_s >> dest_start >> src_start >> count;
            transform.push_back(Triple{dest_start - src_start, {src_start, src_start + count}});
        }
        transforms.push_back(std::move(transform));
    }

    in.close();

    std::vector<Range> ranges = input_ranges;
    for (const std::vector<Triple>& t : transforms) {
        ranges = TransformSet(ranges, t);
    }

    long long answer = std::numeric_limits<long long>::max();
    for (const Range& r : ranges) {
        if (r.end <= r.start) {
            continue;
        }
        answer = std::min(answer, r.start);
    }

    std::cout << answer << std::endl;
    return 0;
}