#include "Dijkstra.h"
#include "BetterPriorityQueue.h"
#include <climits>
#include <stdexcept>

using namespace std;

// for V nodes and E edges, the worst-case time complexity is O(V * E + V * log V)
// - GetNodes and queue initialization take O(V * log V).
// - Outgoing-edge queries scan all E edges for each processed node.
// - Update takes O(V) per examined edge.

// INT_MAX represents unreachable distance. Distances greater
// than or equal to INT_MAX cannot be represented separately.

int dijkstra(nodekey_t start, nodekey_t end, const Graph* g) {
    if (g == nullptr) {
        throw invalid_argument("Graph does not exist");
    }

    if (!g->IsPresent(start) || !g->IsPresent(end)) {
        throw invalid_argument("Start and end nodes do not exist");
    } 

    BetterPriorityQueue queue;

    for (nodekey_t key : g->GetNodes()) {
        BPQNode node;
        node.gnode = key;
        
        if (key == start) {
            node.pri = 0;
        }

        queue.push(node);
    }

    while (!queue.empty()) {
        BPQNode current = queue.top();
        queue.pop();
        if (current.pri == INT_MAX) {
            return INT_MAX;
        }

        if (current.gnode == end) {
            return current.pri;
        }

        for (const GraphEdge* edge : g->GetOutwardEdgesFrom(current.gnode)) {
            BPQNode neighbor;
            neighbor.gnode = edge->to;

            if (edge->weight > static_cast<unsigned int>(INT_MAX - current.pri)) {
                continue;
            }

            neighbor.pri = current.pri + static_cast<int>(edge->weight);

            // Update ignores nodes already removed from the queue.
            queue.Update(neighbor);
        }
    }

    return INT_MAX;
}
