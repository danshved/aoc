#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

struct Set {
    int red = 0;
    int green = 0;
    int blue = 0;
};

struct Game {
    int id = 0;
    std::vector<Set> sets;
};

Set ParseSet(std::string str) {
    Set result;
    std::string cur;
    std::istringstream str_s(str);
    while (std::getline(str_s, cur, ',')) {
        int count;
        std::string kind;
        std::istringstream cur_s(cur);
        cur_s >> count >> kind;
        if (kind == "red") {
            result.red += count;
        } else if (kind == "green") {
            result.green += count;
        } else if (kind == "blue") {
            result.blue += count;
        } else {
            throw "Unknown kind " + kind;
        }
    }
    return result;
}

Game ParseGame(std::string str) {
    Game result;
    std::istringstream str_s(str);

    std::string first;
    std::getline(str_s, first, ':');
    std::string unused;
    std::istringstream(first) >> unused >> result.id;

    std::string cur;
    while (std::getline(str_s, cur, ';')) {
        result.sets.push_back(ParseSet(cur));
    }
    return result;
}

int main() {
    std::vector<Game> games;
    std::ifstream in;
    std::string line;
    in.open("input.txt");
    while (std::getline(in, line)) {
        games.push_back(ParseGame(line));
    }
    in.close();

    int answer = 0;
    for (const Game& game : games) {
        bool ok = true;
        Set t;
        for (const Set& s : game.sets) {
            t.red = std::max(t.red, s.red);
            t.green = std::max(t.green, s.green);
            t.blue = std::max(t.blue, s.blue);
        }
        answer += t.red * t.green * t.blue;
    }

    std::cout << answer << std::endl;
    return 0;
}