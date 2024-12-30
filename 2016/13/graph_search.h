#ifndef __AOC_GRAPH_SEARCH_H__
#define __AOC_GRAPH_SEARCH_H__

#include <algorithm>
#include <cassert>
#include <optional>
#include <queue>
#include <ranges>
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

    int Depth() const {
        return path_.size() - 1;
    }

    std::optional<Node> Parent() {
        return (path_.size() < 2) ? std::nullopt
                                  : std::optional<Node>(path_[path_.size() - 2]);
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

template <typename Node, typename Hasher = std::hash<Node>,
          std::ranges::input_range Range, typename VisitFunc>
DFSResult<Node, Hasher> DFSFrom(const Range& range, VisitFunc&& visit) {
    return DFS<Node, Hasher>(
        [&range](auto& search) {
            for (const Node& node : range) {
                search.Look(node);
            }
        },
        std::forward<VisitFunc>(visit));
}

enum class BFSEdge {
    kTree = 0,
    kTight = 1,
    kLoose = 2,
};

// The result of BFS() is the map containing the depths of all visited nodes.
// Nodes discovered by StartFunc have depth 0, other nodes have depth > 0.
template <typename Node, typename Hasher = std::hash<Node>>
using BFSResult = std::unordered_map<Node, int, Hasher>;

template <typename Node, typename Hasher, typename StartFunc, typename VisitFunc>
class BFSState {
   public:
    BFSEdge Look(const Node& node) {
        int depth = Depth();
        auto [iter, inserted] = depths_.insert(std::make_pair(node, depth + 1));
        if (inserted) {
            queue_.push(node);
            if (current_.has_value()) {
                parents_[node] = *current_;
            }
            return BFSEdge::kTree;
        }
        if (iter->second == depth + 1) {
            return BFSEdge::kTight;
        }
        assert(iter->second <= depth);
        return BFSEdge::kLoose;
    }

    int Depth() const {
        return current_.has_value() ? depths_.at(*current_) : -1;
    }

    std::optional<Node> Parent() const {
        if (!current_.has_value()) {
            return std::nullopt;
        }
        auto it = parents_.find(*current_);
        return (it == parents_.end()) ? std::nullopt
                                      : std::optional<Node>(it->second);
    }

    void Abort() {
        aborted_ = true;
    }

    bool Aborted() const {
        return aborted_;
    }

   private:
    BFSState() = default;

    void Run(StartFunc&& start, VisitFunc&& visit) {
        start(*this);
        while (!queue_.empty() && !aborted_) {
            current_ = queue_.front();
            queue_.pop();
            visit(*this, *current_);
        }
    }

    friend BFSResult<Node, Hasher> BFS<Node, Hasher, StartFunc, VisitFunc>(
        StartFunc&&, VisitFunc&&);

    std::queue<Node> queue_;
    std::unordered_map<Node, int, Hasher> depths_;
    std::unordered_map<Node, Node> parents_;
    std::optional<Node> current_;
    bool aborted_ = false;
};

template <typename Node, typename Hasher = std::hash<Node>,
          typename StartFunc, typename VisitFunc>
BFSResult<Node, Hasher> BFS(StartFunc&& start, VisitFunc&& visit) {
    BFSState<Node, Hasher, StartFunc, VisitFunc> state;
    state.Run(std::forward<StartFunc>(start), std::forward<VisitFunc>(visit));
    return std::move(state.depths_);
}

template <typename Node, typename Hasher = std::hash<Node>,
          typename VisitFunc>
BFSResult<Node, Hasher> BFSFrom(const Node& start, VisitFunc&& visit) {
    return BFS<Node, Hasher>(
        [&start](auto& search) { search.Look(start); },
        std::forward<VisitFunc>(visit));
}

template <typename Node, typename Hasher = std::hash<Node>,
          std::ranges::input_range Range, typename VisitFunc>
BFSResult<Node, Hasher> BFSFrom(const Range& range, VisitFunc&& visit) {
    return BFS<Node, Hasher>(
        [&range](auto& search) {
            for (const Node& node : range) {
                search.Look(node);
            }
        },
        std::forward<VisitFunc>(visit));
}

// The result of Diskstra() is the set of vertices that have been reached.
template <typename Node, typename Dist, typename Hasher = std::hash<Node>>
using DijkstraResult = std::unordered_map<Node, Dist, Hasher>;

template <typename Node, typename Dist>
struct DistUpdate {
    Node node;
    Dist dist;
    std::optional<Node> parent;
    int depth;

    auto operator<=>(const DistUpdate& other) const {
        return other.dist <=> dist;
    }
};

template <typename Node, typename Dist>
class HeapQueue {
   public:
    void Push(const DistUpdate<Node, Dist>& update) {
        queue_.push_back(update);
        std::push_heap(queue_.begin(), queue_.end());
    }

    DistUpdate<Node, Dist> Pop() {
        std::pop_heap(queue_.begin(), queue_.end());
        DistUpdate<Node, Dist> update = std::move(queue_.back());
        queue_.pop_back();
        return update;
    }

    bool Empty() {
        return queue_.empty();
    }

   private:
    std::vector<DistUpdate<Node, Dist>> queue_;
};

// Priority queue for non-negative integer distances that are not much higher
// than the last distance that's been popped.
template <typename Node>
class ShortQueue {
   public:
    void Push(const DistUpdate<Node, int>& update) {
        assert(update.dist >= dist_);
        buckets_[update.dist].push_back(update);
        count_++;
    }

    DistUpdate<Node, int> Pop() {
        assert(!Empty());
        auto it = buckets_.find(dist_);
        while (true) {
            if (it == buckets_.end()) {
                dist_++;
                it = buckets_.find(dist_);
            } else if (it->second.empty()) {
                buckets_.erase(it);
                dist_++;
                it = buckets_.find(dist_);
            } else {
                break;
            }
        }

        DistUpdate<Node, int> update = std::move(it->second.back());
        it->second.pop_back();
        count_--;
        return update;
    }

    bool Empty() {
        return count_ == 0;
    }

   private:
    int dist_ = 0;
    int count_ = 0;
    std::unordered_map<int, std::vector<DistUpdate<Node, int>>> buckets_;
};

template <typename Node, typename Dist, typename Hasher, typename PQueue,
          typename StartFunc, typename VisitFunc>
class DiskstraState {
   public:
    void Look(const Node& node, Dist dist) {
        queue_.Push({
            .node = node,
            .dist = dist,
            .parent = current_.has_value() ? std::optional<Node>(current_->node) : std::nullopt,
            .depth = Depth() + 1,
        });
    }

    int Depth() const {
        return current_.has_value() ? current_->depth : -1;
    }

    std::optional<Node> Parent() const {
        return current_.has_value() ? current_->parent : std::nullopt;
    }

   private:
    DiskstraState() = default;

    void Run(StartFunc&& start, VisitFunc&& visit) {
        start(*this);
        while (!queue_.Empty()) {
            current_ = queue_.Pop();
            auto [_, inserted] = distances_.insert(
                std::make_pair(current_->node, current_->dist));
            if (inserted) {
                visit(*this, current_->node, current_->dist);
            }
        }
    }

    friend DijkstraResult<Node, Dist, Hasher>
    Dijkstra<Node, Dist, Hasher, PQueue, StartFunc, VisitFunc>(
        StartFunc&&, VisitFunc&&);

    PQueue queue_;
    std::unordered_map<Node, Dist, Hasher> distances_;
    std::optional<DistUpdate<Node, Dist>> current_ = std::nullopt;
};

template <typename Node, typename Dist,
          typename Hasher = std::hash<Node>, typename PQueue = HeapQueue<Node, Dist>,
          typename StartFunc, typename VisitFunc>
DijkstraResult<Node, Dist, Hasher> Dijkstra(StartFunc&& start, VisitFunc&& visit) {
    DiskstraState<Node, Dist, Hasher, PQueue, StartFunc, VisitFunc> state;
    state.Run(std::forward<StartFunc>(start), std::forward<VisitFunc>(visit));
    return std::move(state.distances_);
}

template <typename Node, typename Dist, typename Hasher = std::hash<Node>,
          typename VisitFunc>
DijkstraResult<Node, Dist, Hasher>
DijkstraFrom(const Node& start, Dist dist, VisitFunc&& visit) {
    return Dijkstra<Node, Dist, Hasher>(
        [&start, &dist](auto& search) { search.Look(start, dist); },
        std::forward<VisitFunc>(visit));
}

#endif