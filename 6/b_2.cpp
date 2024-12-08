#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <functional>
#include <cassert>

struct Dir {
    int di;
    int dj;
};
const Dir kDirs[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const int kUp = 0;

std::vector<std::string> input;
int height, width;

// Possible states of the guard. There are 4 legal states within each cell
// that is not '#', corresponding to 4 orientations of the guard. There is
// one additional legal state "outside the map".
struct State {
    int i;
    int j;
    int dir;

    bool operator==(const State&) const = default;
};

const State kOutside{0, 0, -1};

// Each legal state has exactly one next state: the one to which the guard will
// transition according to the rules. For convenience, if the guard is outside
// the map, we say that the next state is also outside the map. This way the
// guard always has a next state to transition to, and will always end up in a
// cycle (which may be the kOutside loop or a real cycle).
State Next(const State& s) {
    if (s == kOutside) {
        return kOutside;
    }

    int i1 = s.i + kDirs[s.dir].di;
    int j1 = s.j + kDirs[s.dir].dj;
    if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width) {
        return kOutside;
    }
    if (input[i1][j1] == '#') {
        return State{s.i, s.j, (s.dir + 1) % 4};
    }
    return State{i1, j1, s.dir};
}

// Helper: iterate over all legal states.
void ForEachState(std::function<void(const State&)> func) {
    func(kOutside);
    State s;
    for (s.i = 0; s.i < height; s.i++) {
        for (s.j = 0; s.j < width; s.j++) {
            if (input[s.i][s.j] == '#') {
                continue;
            }
            for (s.dir = 0; s.dir < 4; s.dir++) {
                func(s);
            }
        }
    }
}

// Helper: map with legal states as keys.
template<typename T>
class StateMap {
public:
    void Init(const T& val) {
        _in_values.assign(height, std::vector<std::vector<T>>(width,
                std::vector<T>(4, val)));
        _out_value = val;
    }

    T& operator[](const State& s) {
        return (s == kOutside) ? _out_value : _in_values[s.i][s.j][s.dir];
    }

private:
    std::vector<std::vector<std::vector<T>>> _in_values;
    T _out_value;
};

// There is always exactly one edge going out of every state in the state graph.
// This means that the graph consists of one or more cycles and a bunch of trees
// attached to those cycles. Edges point from leaves towards the root of
// each tree, and that root always belongs to some cycle.
//
// We will do two depth-first-search passes, one forward and one backward.
// This will let us mark up the graph so that we can easily understand where
// each state is located within this tree-and-cycle structure.

// Per-state information from the forward pass.
StateMap<int> color;         // White/gray/black color of the state for depth first search.
StateMap<State> destiny;     // Representative of the only cycle reachable from this state.
StateMap<int> cycle_size;    // If we're in a cycle, the size of that cycle (>= 1). Otherwise 0.
StateMap<int> cycle_pos;     // If we're in a cycle, our position within it.
StateMap<std::vector<State>> reverse;

void VisitForward(const State& u) {
    color[u] = 1;

    State v = Next(u);
    reverse[v].push_back(u);
    if (color[v] == 0) {
        // Never seen state v before. Go into it recursively.
        VisitForward(v);
        destiny[u] = destiny[v];
    } else if (color[v] == 1) {
        // We found a cycle. State u becomes its representative.
        destiny[u] = u;

        int pos = 0;
        cycle_pos[u] = pos++;
        for (State x = Next(u); x != u; x = Next(x)) {
            cycle_pos[x] = pos++;
        }

        cycle_size[u] = pos;
        for (State x = Next(u); x != u; x = Next(x)) {
            cycle_size[x] = pos;
        }
    } else {
        // State v has already been visited and its destiny is known.
        destiny[u] = destiny[v];
    }

    color[u] = 2;
}

// Per-state information from the backward pass.
StateMap<State> root;   // Root of the tree that the state belongs to.
StateMap<int> depth;    // Distance from the state to the root of the tree.
StateMap<int> enter;    // Time when the state was entered in the reverse search.
StateMap<int> leave;    // Time when the state was exited in the reverse search.

void VisitBackward(const State& u, const State& r, int d) {
    static int time = 0;
    root[u] = r;
    depth[u] = d;
    enter[u] = ++time;
    for (const State& v : reverse[u]) {
        if (cycle_size[v] == 0) {
            VisitBackward(v, r, d + 1);
        }
    }
    leave[u] = ++time;
}

const int kInf = std::numeric_limits<int>::max();

// Determine whether v is reachable from u, and in how many steps.
int GetDistance(const State& u, const State& v) {
    if (destiny[u] != destiny[v]) {
        return kInf;
    }
    if (cycle_size[v] != 0) {
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
        // u and v are in different trees / branches.
        return kInf;
    }
}

int main() {
    std::ifstream in("input.txt");
    std::string line;
    while (std::getline(in, line)) {
        input.push_back(std::move(line));
    }
    height = input.size();
    width = input[0].size();

    // Forward depth-first search. 
    color.Init(0);
    destiny.Init(kOutside);
    cycle_size.Init(0);
    cycle_pos.Init(0);
    reverse.Init(std::vector<State>());
    ForEachState([](const State& u) {
        if (color[u] == 0) {
            VisitForward(u);
        }
    });

    // Backward depth-first search starting from each in-cycle state and
    // descending into the attached trees.
    root.Init(kOutside);
    depth.Init(0);
    enter.Init(0);
    leave.Init(0);
    ForEachState([](const State& u) {
        if (cycle_size[u] != 0) {
            VisitBackward(u, u, 0);
        }
    });

    // Find the guard.
    State start = kOutside;
    for (int i = 0; i < height; ++i) {
        std::string::size_type pos = input[i].find('^');
        if (pos != std::string::npos) {
            start = State{i, static_cast<int>(pos), kUp};
            break;
        }
    }
    assert(start != kOutside);

    // Try every obstacle position.
    int answer = 0;
    for (int obs_i = 0; obs_i < height; obs_i++) {
        for (int obs_j = 0; obs_j < width; obs_j++) {
            if (input[obs_i][obs_j] != '.') {
                continue;
            }

            State guard = start;
            std::vector<bool> seen(4, false);

            while(true) {
                // Jump to the next time the guard hits the obstacle, i.e.
                // tries to enter into one of the 4 states in the obstacle cell.
                State hit = kOutside;
                int dist = kInf;
                for (int dir = 0; dir < 4; dir++) {
                    State v{obs_i, obs_j, dir};
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
                if (seen[hit.dir]) {
                    answer++;
                    break;
                } else {
                    seen[hit.dir] = true;
                }

                // Instead of walking through the obstacle, stop in front of it
                // and rotate 90 degrees.
                guard = State{hit.i - kDirs[hit.dir].di, hit.j - kDirs[hit.dir].dj, (hit.dir + 1) % 4};
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}