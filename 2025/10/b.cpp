#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
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

int kInf = std::numeric_limits<int>::max();

int main() {
    int answer = 0;
    int case_no = 0;
    for(const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        int n = 0;
        std::vector<std::vector<int>> buttons;
        std::vector<int> goal;
        for (const std::string& word : Split(line, " ")) {
            assert (word.size() >= 2);
            std::string inside = word.substr(1, word.size() - 2);
            switch(word[0]) {
                case '[': {
                    n = inside.size();
                    break;
                }
                case '(': {
                    std::vector<int> button(n, int{0});
                    for (const std::string& s : Split(inside, ",")) {
                        button[std::stoi(s)] = 1;
                    }
                    buttons.push_back(button);
                    break;
                }
                case '{': {
                    for (const std::string& s : Split(inside, ",")) {
                        goal.push_back(int{std::stoi(s)});
                    }
                    assert(goal.size() == n);
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }
        }
        std::cout << "\nStarting case " << (++case_no) << "\n\n" << n << "\n" << FormatVector(goal, " ") << std::endl;
        for (const std::vector<int>& button : buttons) {
            std::cout << FormatVector(button, " ") << "\n";
        }

        std::sort(buttons.begin(), buttons.end(), [&](const std::vector<int>& a, const std::vector<int>&b) {
            int wa = 0, wb = 0;
            for (int i = 0; i < n; i++) {
                wa += a[i];
                wb += b[i];
            }
            return wa > wb;
        });
        std::cout << "\nSorted:\n";
        for (const std::vector<int>& button : buttons) {
            std::cout << FormatVector(button, " ") << "\n";
        }

        NestedVector<2, bool> covers = ConstVector(false, buttons.size() + 1, n);
        for (int start_button = buttons.size() - 1; start_button >= 0; start_button--) {
            for (int i = 0; i < n; i++) {
                covers[start_button][i] = covers[start_button + 1][i] || (buttons[start_button][i] != 0);
            }
        }

        auto best = [&](auto&& self, int start_button, int max_presses, std::vector<int> cur_goal) -> int {
            if (start_button >= buttons.size() || max_presses == 0) {
                return std::ranges::all_of(cur_goal, [](int x) {return x == 0;}) ? 0 : kInf;
            }

            for (int i = 0; i < n; i++) {
                if (cur_goal[i] == 0) {
                    continue;
                }
                if (!covers[start_button][i]) {
                    return kInf;
                }
                if (!covers[start_button + 1][i]) {
                    int start_presses = cur_goal[i];
                    if (start_presses > max_presses) {
                        return kInf;
                    }
                    for (int light = 0; light < n; light++) {
                        cur_goal[light] -= buttons[start_button][light] * start_presses;
                        if (cur_goal[light] < 0) {
                            return kInf;
                        }
                    }
                    int found = self(self, start_button + 1, max_presses - start_presses, cur_goal);
                    return (found == kInf) ? kInf : (start_presses + found);
                }
            }


            int best_found = self(self, start_button + 1, max_presses, cur_goal);
            max_presses = std::min(max_presses, best_found);

            for (int start_presses = 1; start_presses <= max_presses; start_presses++) {
                int light;
                for (light = 0; light < n; light++) {
                    cur_goal[light] -= buttons[start_button][light];
                    if (cur_goal[light] < 0) {
                        break;
                    }
                }
                if (light < n) {
                    break;
                }
                int found = self(self, start_button + 1, max_presses - start_presses, cur_goal);
                if (found != kInf) {
                    best_found = start_presses + found;
                    max_presses = best_found;
                }
            }
            return best_found;
        };

        int presses = best(best, 0, 1000, goal);
        std::cout << "Best presses: " << presses << std::endl;
        answer += presses;
    }

    std::cout << answer << std::endl;
    return 0;
}
