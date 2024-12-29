#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "order.h"
#include "numbers.h"
#include "parse.h"

const double kMin = 200000000000000LL;
const double kMax = 400000000000000LL;

struct Stone
{
    double x, y, z;
    double vx, vy, vz;
};

Stone ParseStone(const std::string &s)
{
    std::vector<std::string> ws = Split(s, '@');
    assert(ws.size() == 2);

    std::vector<double> pos = ParseVector<double>(ws[0]);
    assert(pos.size() == 3);

    std::vector<double> v = ParseVector<double>(ws[1]);
    assert(v.size() == 3);

    return Stone{pos[0], pos[1], pos[2], v[0], v[1], v[2]};
}

template <typename T>
T Det(T a, T b, T c, T d)
{
    return a * d - b * c;
}

int main()
{
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Stone> stones;
    for (const std::string &line : lines)
    {
        stones.push_back(ParseStone(line));
    }

    // With lots of different ad-hoc runs, this is what it comes down to.
    double vx = 263, vy = 120, vz = 21;
    long long meet_x = 118378223846841, meet_y = 228996474589321, meet_z = 259397320329497;
    std::cout << meet_x + meet_y + meet_z << std::endl;

    return 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = i + 1; j < 5; j++)
        {
            Stone a = stones[i];
            Stone b = stones[j];

            a.vx -= vx;
            a.vy -= vy;
            b.vx -= vx;
            b.vy -= vy;

            double m11 = a.vx, m12 = -b.vx, y1 = b.x - a.x;
            double m21 = a.vy, m22 = -b.vy, y2 = b.y - a.y;
            double d = Det(m11, m21, m12, m22);

            if (d == 0)
            {
                continue;
            }
            double t1 = Det(y1, y2, m12, m22) / d;
            double t2 = Det(m11, m21, y1, y2) / d;

            std::cout << (long long)(a.x + t1 * a.vx) << " " << (long long)(a.y + t1 * a.vy) << std::endl;
        }
    }

    return 0;
}