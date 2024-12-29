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

int main() {
    std::string line1, line2;
    std::tie(line1, line2) = Split2(Trim(GetContents("input.txt")), '\n');

    long long t = std::stoll((line1));
    long long d = std::stoll((line2));

    long long count = 0;
    for (long long j = 0; j <= t; ++j) {
        if (j * (t - j) > d) {
            count++;
        }
    }
    std::cout << count << std::endl;
    
    return 0;
}