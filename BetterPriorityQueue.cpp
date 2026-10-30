#include "BetterPriorityQueue.h"
#include <sstream>
#include <utility>

bool BetterPriorityQueue::Contains(const BPQNode& node) const {
    for (const BPQNode& current : this->c) {
        // == compares graph-node keys, not priorities or visited flags.
        if (current == node) {
            return true;
        }
    }
    return false;
}

bool BetterPriorityQueue::Update(const BPQNode& node) {
    for (size_t i = 0; i < this->c.size(); ++i) {
        if (this->c[i] == node) {
            if (node.pri >= this->c[i].pri) {
                return false;
            }

            // change only the priority, the dummy input's other fields do
            // not need to describe the stored node's visited state.
            this->c[i].pri = node.pri;

            // decreasing a priority can only break the ordering with a parent.
            // in a zero-based heap, index i's parent is (i - 1) / 2.
            while (i > 0) {
                size_t parent = (i - 1) / 2;
                if (!this->comp(this->c[parent], this->c[i])) {
                    break;
                }
                std::swap(this->c[parent], this->c[i]);
                i = parent;
            }
            return true;
        }
    }
    return false;
}

std::string BetterPriorityQueue::ToString() const {
    std::ostringstream result;
    result << "[";
    // printing the underlying heap preserves the vector format
    // without removing any items from the queue.
    for (size_t i = 0; i < this->c.size(); ++i) {
        if (i != 0) {
            result << ", ";
        }
        result << BPQNodeToString(this->c[i]);
    }
    result << "]";
    return result.str();
}

std::string BetterPriorityQueue::BPQNodeToString(const BPQNode& node) {
    return "(" + std::to_string(node.gnode) + ", pri: "
           + std::to_string(node.pri) + ")";
}
