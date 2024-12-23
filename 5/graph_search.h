#ifndef __AOC_GRAPH_SEARCH_H__
#define __AOC_GRAPH_SEARCH_H__

#include <vector>

enum class DFSLook { kVisited, kSkippedBack, kSkippedOld };

template <typename Node, typename VisitFunc>
class DFSState {
   public:
    DFSState(VisitFunc& visit) : visit_(visit) {}

    DFSLook Look(const Node& node) {
        switch (colors_[node]) {
        case 0:
            colors_[node] = 1;
            path_.push_back(node);
            visit_(*this, node);
            path_.pop_back();
            colors_[node] = 2;
            return DFSLook::kVisited;
        case 1:
            return DFSLook::kSkippedBack;
        case 2:
            return DFSLook::kSkippedOld;
        default:
            assert(false);
        }
    }

    const std::vector<Node>& Path() {
        return path_;
    }
    
   private:
    VisitFunc& visit_;
    std::unordered_map<Node, int> colors_;
    std::vector<Node> path_;
};

template <typename Node, typename StartFunc, typename VisitFunc>
void DFS(StartFunc&& start, VisitFunc&& visit) {
    DFSState<Node, VisitFunc> state(visit);
    start(state);
}

// TODO: add support for custom hashers.

#endif