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

struct Card {
    std::vector<int> winning;
    std::vector<int> actual;
};

Card ParseCard(const std::string& s) 
{
    std::istringstream iss(s);
    std::string line1, line2, line3;
    std::getline(iss, line1, ':');
    std::getline(iss, line2, '|');
    std::getline(iss, line3);

    Card result;

    std::istringstream line1_s(line2);
    std::copy(std::istream_iterator<int>(line1_s),
            std::istream_iterator<int>(), std::back_inserter(result.winning));

    std::istringstream line2_s(line3);
    std::copy(std::istream_iterator<int>(line2_s),
            std::istream_iterator<int>(), std::back_inserter(result.actual));

    return result;
}

std::vector<Card> cards;

int main() {
    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line)) {
        cards.push_back(ParseCard(line));
    }
    in.close();

    int n = cards.size();
    std::vector<int> counts(n, 1);
    int answer = 0;

    for (int i = 0; i < n; ++i) {
        std::unordered_set<int> winning_set(cards[i].winning.begin(), cards[i].winning.end());
        int count = 0;
        for (int x : cards[i].actual) {
            if (winning_set.find(x) != winning_set.end()) {
                ++count;
            }
        }

        for (int d = 1; d <= count; d++) {
            counts[i + d] += counts[i];
        }

        answer += counts[i];
    }


    std::cout << answer << std::endl;
    return 0;
}