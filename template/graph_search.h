#ifndef __AOC_GRAPH_SEARCH_H__
#define __AOC_GRAPH_SEARCH_H__

#include <unordered_map>
#include <vector>

enum class DFSEdge {
    kTree,
    kBack,
    kForward,
    kCross,
};

template <typename Node, typename Hasher = std::hash<Node>>
struct DFSResult {
    std::unordered_map<Node, int, Hasher> enter_times;
    std::unordered_map<Node, int, Hasher> exit_times;
};

template <typename Node, typename Hasher, typename StartFunc, typename VisitFunc>
class DFSState {
   public:
    // Tell the search to look at `node`. This means that logically there is
    // an edge going from the current node to `node`.
    //
    // Returns the edge class of this logical edge w.r.t. the DFS forest.
    DFSEdge Look(const Node& node) {
        auto [enter_it, inserted] = enter_times_.insert(std::make_pair(node, time_));
        if (inserted) {
            time_++;
            path_.push_back(node);
            visit_(*this, node);
            path_.pop_back();
            exit_times_.insert(std::make_pair(node, time_));
            time_++;
            return DFSEdge::kTree;
        }

        auto exit_it = exit_times_.find(node);
        if (exit_it == exit_times_.end()) {
            return DFSEdge::kBack;
        }

        if (!path_.empty() && exit_it->second < enter_times_.at(path_.back())) {
            return DFSEdge::kCross;
        }
        return DFSEdge::kForward;
    }

    const std::vector<Node>& Path() const {
        return path_;
    }

   private:
    DFSState(VisitFunc& visit) : visit_(visit) {}

    friend DFSResult<Node, Hasher> DFS<Node, Hasher, StartFunc, VisitFunc>(
        StartFunc&&, VisitFunc&&);

    VisitFunc& visit_;
    int time_ = 0;
    std::unordered_map<Node, int, Hasher> enter_times_;
    std::unordered_map<Node, int, Hasher> exit_times_;
    std::vector<Node> path_;
};

template <typename Node, typename Hasher = std::hash<Node>,
          typename StartFunc, typename VisitFunc>
DFSResult<Node, Hasher> DFS(StartFunc&& start, VisitFunc&& visit) {
    DFSState<Node, Hasher, StartFunc, VisitFunc> state(visit);
    start(state);
    return {
        .enter_times = std::move(state.enter_times_),
        .exit_times = std::move(state.exit_times_),
    };
}

template <typename Node, typename Hasher = std::hash<Node>,
          typename VisitFunc>
DFSResult<Node, Hasher> DFSFrom(const Node& start, VisitFunc&& visit) {
    return DFS<Node, Hasher>(
        [&start](auto& search) { search.Look(start); },
        std::forward<VisitFunc>(visit));
}

#endif