#ifndef __AOC_COLLECTIONS_H__
#define __AOC_COLLECTIONS_H__

#include <cassert>
#include <vector>
#include <iterator>

// Transposes a matrix. Could be a std::vector<std::vector<T>>
// or std::vector<std::string>.
template<typename C>
std::vector<C> Transpose(const std::vector<C>& v) {
    int height = v.size();
    assert (height > 0);
    int width = v[0].size();
    assert (width > 0);
    for (int i = 0; i < height; i++) {
        assert (v[i].size() == width);
    }

    std::vector<C> result;
    result.reserve(width);
    for (int j = 0; j < width; j++) {
        C result_row;
        result_row.reserve(height);
        for (int i = 0; i < height; i++) {
            result_row.push_back(v[i][j]);
        }
        result.push_back(std::move(result_row));
    }
    return result;
}

// Concatenates a vector of containers into a single container of the same type.
template<typename C>
C Concat(const std::vector<C>& v) {
    C result;
    for (const C& c : v) {
        std::copy(std::begin(c), std::end(c), std::back_inserter(result));
    }
    return result;
}

#endif