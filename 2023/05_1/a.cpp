#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iterator>
#include <limits>
#include <cctype>
#include <cassert>

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

// Format vector for debugging.
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

// Split a container into a vector of containers.
template<typename T, typename C>
std::vector<C> SplitImpl(const C& xs, const T& delim) {
    std::vector<C> result;
    C current;
    for (const T& x : xs) {
        if (x == delim) {
            result.push_back(std::move(current));
            current.clear();
        } else {
            current.push_back(x);
        }
    }
    result.push_back(std::move(current));
    return result;
}

// Splits a string using the given delimiter. The resulting vector's size
// is always one plus the number of delimiters in the string. If the string is
// empty, the result is the vector containing one empty string.
std::vector<std::string> Split(const std::string& s, char delim) {
    return SplitImpl<char, std::string>(s, delim);
}

// The version of Split() for vectors instead of strings.
template<typename T>
std::vector<std::vector<T>> Split(const std::vector<T>& xs, const T& delim) {
    return SplitImpl<T, std::vector<T>>(xs, delim);
}

template<typename T, typename C>
std::pair<C, C> Split2Impl(const C& xs, const T& delim) {
    std::pair<C, C> result;
    bool found_delimiter = false;
    for (const T& x : xs) {
        if (found_delimiter) {
            result.second.push_back(x);
        } else if (x == delim) {
            found_delimiter = true;
        } else {
            result.first.push_back(x);
        }
    }
    assert(found_delimiter);
    return result;
}

// Splits the string at the first occurrence of `delim`. Fails with assert() if
// `delim` does not appear in the string, including the case if the string is
// empty.
std::pair<std::string, std::string> Split2(const std::string& s, char delim) {
    return Split2Impl<char, std::string>(s, delim);
}

// The version of Split2() for vectors.
template<typename T>
std::pair<std::vector<T>, std::vector<T>> Split2(const std::vector<T>& xs, const T& delim) {
    return Split2Impl<T, std::vector<T>>(xs, delim);
}

// Removes whitespace in the beginning and end of a string.
std::string Trim(const std::string& s) {
    int i = 0, j = s.length();
    for (; i < j && std::isspace(s[i]); i++) {};
    for(; i < j && std::isspace(s[j - 1]); j--) {};
    return s.substr(i, j - i);
}

// Reads entire text file into a string. Newlines in the output are plain '\n'.
std::string GetContents(const std::string& filename) {
    std::ifstream f(filename);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

struct Range {
    long long begin;
    long long end;
};

struct Triple {
    Range range;
    long long shift;

    bool operator<(const Triple& other) const {
        return range.begin < other.range.begin;
    }
};

std::vector<Range> Transform(const std::vector<Range>& ranges, std::vector<Triple> ts) {
    std::sort(ts.begin(), ts.end());
    std::vector<Range> result;
    for (Range range : ranges) {
        for (const Triple& t : ts) {
            // Left
            long long b = range.begin;
            long long e = std::min(range.end, t.range.begin);
            if (b < e) {
                result.push_back(Range{b, e});
            }

            // Intersection
            b = std::max(range.begin, t.range.begin);
            e = std::min(range.end, t.range.end);
            if (b < e) {
                result.push_back(Range{b + t.shift, e + t.shift});
            }

            // Right
            range.begin = std::max(range.begin, t.range.end);
            if (range.begin >= range.end) {
                break;
            }
        }
        if (range.begin < range.end) {
            result.push_back(range);
        }
    }
    return result;
}

int main() {
    std::string data = Trim(GetContents("input.txt"));
    std::vector<std::string> lines = Split(data, '\n');
    std::vector<std::vector<std::string>> pars = Split(lines, std::string());

    std::vector<long long> xs = ParseVector<long long>(Split2(pars[0][0], ':').second);
    std::vector<Range> ranges;
    for (int i = 0; i < xs.size() / 2; i++) {
        ranges.push_back(Range{xs[2 * i], xs[2 * i] + xs[2 * i + 1]});
    }

    std::vector<std::vector<Triple>> transforms;
    for (int p = 1; p < pars.size(); p++) {
        std::vector<Triple> ts;
        for (int l = 1; l < pars[p].size(); l++) {
            long long dst, src, cnt;
            std::istringstream(pars[p][l]) >> dst >> src >> cnt;
            ts.push_back(Triple{{src, src + cnt}, dst - src});
        }
        transforms.push_back(std::move(ts));
    }

    for(const std::vector<Triple>& t : transforms) {
        ranges = Transform(ranges, t);
    }

    long long answer = std::numeric_limits<long long>::max();
    for (const Range& range : ranges) {
        if (range.end > range.begin) {
            answer = std::min(answer, range.begin);
        }
    }

    std::cout << answer << std::endl;
    
    return 0;
}