#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"
#include "graph_search.h"

// Possible states (position + direction) of the guard. There are 4 legal states
// within each cell that is not '#'. There is one additional legal state
// "outside the map".
using State = PosDir;
const State kOutside = {{0, 0}, {0, 0}};

std::vector<std::string> input;
int size_i, size_j;

// Each legal state has exactly one next state: the one to which the guard will
// transition according to the rules. For convenience, if the guard is outside
// the map, we say that the next state is also outside the map. This way the
// guard always has a next state to transition to, and will always end up in a
// cycle (which may be the kOutside loop or a real cycle).
State Next(const State& s) {
    if (s == kOutside) {
        return kOutside;
    }

    State next = s.Step();
    return (!InBounds(next.pos, size_i, size_j))    ? kOutside
           : (input[next.pos.i][next.pos.j] == '#') ? s.RotateRight()
                                                    : next;
}

// Helper: map whose keys are states.
template <typename T>
using StateMap = std::unordered_map<State, T>;

// There is always exactly one edge going out of every state in the state graph.
// This means that the graph consists of one or more cycles and a bunch of trees
// attached to those cycles. Edges point from leaves towards the root of
// each tree, and that root always belongs to some cycle.
//
// We will do two depth-first-search passes, one forward and one backward.
// This will let us mark up the graph so that we can easily understand where
// each state is located within this tree-and-cycle structure.

// Per-state information from the forward pass.
StateMap<State> destiny;   // Representative of the only cycle reachable from this state.
StateMap<int> cycle_size;  // If we're in a cycle, the size of that cycle (>= 1).
StateMap<int> cycle_pos;   // If we're in a cycle, our position within it.
StateMap<std::vector<State>> reverse;

// Per-state information from the backward pass.
StateMap<State> root;  // Root of the tree that the state belongs to.
StateMap<int> depth;   // Distance from the state to the root of the tree.
StateMap<int> enter;   // Time when the state was entered in the reverse search.
StateMap<int> leave;   // Time when the state was exited in the reverse search.

const int kInf = std::numeric_limits<int>::max();

// Determine whether v is reachable from u, and in how many steps.
int GetDistance(const State& u, const State& v) {
    if (destiny[u] != destiny[v]) {
        return kInf;
    }
    if (cycle_size.contains(v)) {
        // The simple path is u -> root[u] up the tree and then
        // root[u] -> v along the cycle.
        int pos1 = cycle_pos[root[u]];
        int pos2 = cycle_pos[v];
        int length = cycle_size[v];
        return depth[u] + (pos2 - pos1 + length) % length;
    } else if (enter[v] <= enter[u] && leave[u] <= leave[v]) {
        // The simple path is from u to v up the tree.
        return depth[u] - depth[v];
    } else {
        // u is not a descendant of v.
        return kInf;
    }
}

int main() {
    input = Split(Trim(GetContents("input.txt")), "\n");
    std::tie(size_i, size_j) = Sizes<2>(input);

    // Forward depth-first search.
    DFS<State>(
        [](auto& search) {
            for (Coord pos : Bounds(size_i, size_j)) {
                if (input[pos.i][pos.j] == '#') {
                    continue;
                }
                for (Coord dir : kDirs) {
                    search.Look({pos, dir});
                }
            }
            search.Look(kOutside);
        },
        [](auto& search, const State& u) {
            State v = Next(u);
            if (search.Look(v) == DFSEdge::kBack) {
                // We found a cycle. State u becomes its representative.
                destiny[u] = u;

                auto begin = std::ranges::find(search.Path(), v);
                auto end = search.Path().end();
                for (auto it = begin; it != end; it++) {
                    cycle_size[*it] = end - begin;
                    cycle_pos[*it] = it - begin;
                }
            } else {
                destiny[u] = destiny[v];
            }
            reverse[v].push_back(u);
        });

    // Backward depth-first search starting from each in-cycle state and
    // descending into the attached trees.
    DFSResult<State> res = DFS<State>(
        [](auto& search) {
            for (const State& u : cycle_size | std::views::keys) {
                search.Look(u);
            }
        },
        [](auto& search, const State& u) {
            root[u] = search.Path().front();
            depth[u] = search.Path().size() - 1;
            for (const State& v : reverse[u]) {
                if (!cycle_size.contains(v)) {
                    search.Look(v);
                }
            }
        });
    enter = std::move(res.enter_times);
    leave = std::move(res.exit_times);

    // Find the guard.
    auto [i, j] = FindOrDie<2>(input, '^');
    State start = {{i, j}, kNorth};

    // Try every obstacle position.
    int answer = 0;
    for (Coord obs : Bounds(size_i, size_j)) {
        if (input[obs.i][obs.j] != '.') {
            continue;
        }

        State guard = start;
        std::unordered_set<Coord> seen;
        while (true) {
            // Jump to the next time the guard hits the obstacle, i.e.
            // tries to enter into one of the 4 states in the obstacle cell.
            State hit = kOutside;
            int dist = kInf;
            for (Coord dir : kDirs) {
                State v = {obs, dir};
                int v_dist = GetDistance(guard, v);
                if (v_dist < dist) {
                    dist = v_dist;
                    hit = v;
                }
            }

            // If the obstacle is unreachable from the guard's current state,
            // the guard ends up in a cycle away from the obstacle.
            if (dist == kInf) {
                if (destiny[guard] != kOutside) {
                    answer++;
                }
                break;
            }

            // If we already hit this side of the obstacle before, this is a cycle.
            if (auto [_, inserted] = seen.insert(hit.dir); !inserted) {
                answer++;
                break;
            }

            // Instead of walking through the obstacle, stop in front of it
            // and rotate 90 degrees.
            guard = hit.StepBack().RotateRight();
        }
    }

    std::cout << answer << std::endl;
    return 0;
}