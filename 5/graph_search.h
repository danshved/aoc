#ifndef __AOC_GRAPH_SEARCH_H__
#define __AOC_GRAPH_SEARCH_H__

#include <vector>

enum class DFSColor {
    kNew = 0,
    kOnPath = 1,
    kOld = 2,
};

template <typename Node, typename Hasher, typename VisitFunc>
class DFSState {
   public:
    DFSState(VisitFunc& visit) : visit_(visit) {}

    // Tell the search to look at `node`. This means that logically there is
    // an edge going from the current node to `node`.
    //
    // Returns the DFS color of `node` at the time when Look() was called.
    // If this is `kOld` or `kOnPath`, Look() has been a no-op.
    // If this is kNew, Look() has followed the edge into `node` recursively.
    DFSColor Look(const Node& node) {
        DFSColor& color = colors_[node];
        if (color == DFSColor::kNew) {
            color = DFSColor::kOnPath;
            path_.push_back(node);
            visit_(*this, node);
            path_.pop_back();
            colors_[node] = DFSColor::kOld;
            return DFSColor::kNew;
        }
        return color;
    }

    const std::vector<Node>& Path() {
        return path_;
    }

   private:
    VisitFunc& visit_;
    std::unordered_map<Node, DFSColor, Hasher> colors_;
    std::vector<Node> path_;
};

template <typename Node, typename Hasher = std::hash<Node>,
          typename StartFunc, typename VisitFunc>
void DFS(StartFunc&& start, VisitFunc&& visit) {
    DFSState<Node, Hasher, VisitFunc> state(visit);
    start(state);
}

#endif