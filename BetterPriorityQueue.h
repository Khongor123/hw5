#ifndef BETTER_PRIORITY_QUEUE_H
#define BETTER_PRIORITY_QUEUE_H

#include "Graph.h"
#include <functional>
#include <queue>
#include <string>
#include <vector>

// BPQNode placed here so queue users
// share the provided node representation and comparison rules
struct BPQNode {
    int pri = INT_MAX;
    bool visited = false;
    nodekey_t gnode = INVALID_NODE_KEY;

    bool operator<(const BPQNode& other) const {
        return pri < other.pri;
    }

    bool operator>(const BPQNode& other) const {
        return pri > other.pri;
    }

    bool operator==(const BPQNode& other) const {
        return gnode == other.gnode;
    }
};

class BetterPriorityQueue: public std::priority_queue<BPQNode, std::vector<BPQNode>, std::greater<BPQNode>> {
public:
    bool Contains(const BPQNode& node) const;
    bool Update(const BPQNode& node);
    std::string ToString() const;
    static std::string BPQNodeToString(const BPQNode& node);
};

#endif
